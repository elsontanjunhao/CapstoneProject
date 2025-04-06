#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2
from std_msgs.msg import Bool
from transformers import Qwen2VLForConditionalGeneration, AutoTokenizer, AutoProcessor, Qwen2_5_VLForConditionalGeneration, BitsAndBytesConfig
from qwen_vl_utils import process_vision_info
import torch
import sys


class VLMNode(Node):
    def __init__(self):
        super().__init__('vlm_node')
        self.get_logger().info("Entered VLM node")

        # ROS2 subscriber to receive the detected obstacle image
        self.create_subscription(Image,'det_obs_img',self.image_callback,10)

        self.det_obs_ack_pub = self.create_publisher(Bool, '/det_obs_ack', 10)
        self.timer = self.create_timer(1, self.pub_img_callback)
        
        # Initialize bridge for ROS <-> OpenCV image conversion
        self.bridge = CvBridge()

        bnb_config = BitsAndBytesConfig(
            load_in_8bit=True,
            llm_int8_threshold=6.0
        )

        # VLM model initialization  
        self.model = Qwen2VLForConditionalGeneration.from_pretrained(
            "Qwen/Qwen2-VL-2B-Instruct",
            torch_dtype=torch.bfloat16, #auto
            device_map="auto",
            #quantization_config=bnb_config
        )
        # self.model = Qwen2_5_VLForConditionalGeneration.from_pretrained(
        #     "Qwen/Qwen2.5-VL-3B-Instruct", torch_dtype="auto", device_map="auto"
        # )
        self.get_logger().info("Finish VLM init")
        self.processor = AutoProcessor.from_pretrained("Qwen/Qwen2-VL-2B-Instruct")
        # self.processor = AutoProcessor.from_pretrained("Qwen/Qwen2.5-VL-3B-Instruct")
        self.latest_image = None
        self.processed_image = False

    def pub_img_callback(self):
        ack_msg = Bool(data=True)
        if self.latest_image is None:
            self.get_logger().warn("No image received yet to process")
            return
        else:
            self.det_obs_ack_pub.publish(ack_msg)
            self.get_logger().info("Received final image from sam_node.py, acknowledging")
            if not self.processed_image:
                self.process_image_with_vlm()
            else:
                self.get_logger().info("Obstacle traversability information concluded! AI model shutting down!")
                self.get_logger().info("Shutting down node...")
                # Cancel the timer
                self.timer.cancel()
                # Destroy publishers and subscribers
                self.det_obs_ack_pub.destroy()
                raise SystemExit



    def image_callback(self, msg):
        """Callback to receive and store the image from the 'det_obs_img' topic."""
        self.latest_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding="bgr8")


    def process_image_with_vlm(self):
        """Process the latest image with the visual language model."""

 

        # Save the image temporarily for inference
        temp_image_path = "/tmp/latest_obstacle_image.jpg"
        cv2.imwrite(temp_image_path, self.latest_image)
        if temp_image_path:
            self.get_logger().info("Processing text output, Please wait...")
        else:
            self.get_logger().info("Cannot process text output, image not found!")
        self.processed_image = True

        # Prepare messages for VLM inference
        messages = [
            {
                "role": "user",
                "content": [
                    {"type": "image", "image": temp_image_path},
                    {"type": "text", "text": "Analyze the object inside the red bounding box and answer the following questions concisely: 1) Identify the object inside the red bounding box. Provide a short label or name. 2) Describe its hardness and solidity using one of these options: Solid, Soft. 3) Compare the object's size to an autonomous mobile robot and choose one of these options: Big, Medium, Small. 4) Determine its level of traversability for an autonomous mobile robot's navigation using one of these options: Not traversable, Possible to traverse, Traversable."}, 
                ],
            }
        ]

        # Process and run inference
        text = self.processor.apply_chat_template(
            messages, tokenize=False, add_generation_prompt=True
        )
        image_inputs, video_inputs = process_vision_info(messages)
        inputs = self.processor(
            text=[text],
            images=image_inputs,
            videos=video_inputs,
            padding=True,
            return_tensors="pt",
        )
        inputs = inputs.to("cuda")
        generated_ids = self.model.generate(**inputs, max_new_tokens=128)
        generated_ids_trimmed = [
            out_ids[len(in_ids):] for in_ids, out_ids in zip(inputs.input_ids, generated_ids)
        ]
        output_text = self.processor.batch_decode(
            generated_ids_trimmed, skip_special_tokens=True, clean_up_tokenization_spaces=False
        )
        print(output_text)
        

def main(args=None):
    try:
        rclpy.init(args=args)
        node = VLMNode()
        try:
            rclpy.spin(node)
        except SystemExit:                 # <--- process the exception 
            rclpy.logging.get_logger("vlm_node").info('System shutdown')
        finally:
            node.destroy_node()
            rclpy.shutdown()
            sys.exit(0)
    except Exception as e:
        print(f"An error occurred: {e}")
        sys.exit(1)
if __name__ == "__main__":
    main()


