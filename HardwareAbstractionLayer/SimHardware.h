#ifndef SIM_HARDWARE_H
#define SIM_HARDWARE_H

#include "HardwareInterfaces.h"
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <std_msgs/msg/float64.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <sensor_msgs/msg/image.hpp>

#define ULTRASONIC_THRESHOLD_MIN 0.02
#define ULTRASONIC_THRESHOLD_MAX 3.0
#define ULTRASONIC_NO_OBJECT_DISTANCE -1.0

using namespace std;


class SimVisionVoyager : public IVisionVoyagerInterface {

private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr pan_pub;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr tilt_pub;
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr ultrasonic_sub;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_gs_left;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_gs_center;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_gs_right;

    geometry_msgs::msg::Twist last_cmd;
    std_msgs::msg::Float64  last_pan;
    std_msgs::msg::Float64  last_tilt;

    float last_ultrasonic_distance = 0.0;
    vector<int> last_grayscale_values = {255, 255, 255};

    void setup_ultrasonic_communication(rclcpp::Node::SharedPtr node)
    {
        ultrasonic_sub = node->create_subscription<sensor_msgs::msg::LaserScan>("/ultrasonic/scan", 10, 
        [this](const sensor_msgs::msg::LaserScan::SharedPtr msg) 
        {
            if (!msg->ranges.empty()) 
            {
                float min_dist = *std::min_element(msg->ranges.begin(), msg->ranges.end());

                if (min_dist > ULTRASONIC_THRESHOLD_MIN && min_dist < ULTRASONIC_THRESHOLD_MAX) 
                {
                    this->last_ultrasonic_distance = min_dist;
                } 
                else
                {
                    this->last_ultrasonic_distance = ULTRASONIC_NO_OBJECT_DISTANCE;
                }
            }
        });
    }


    void setup_grayscale_communication(rclcpp::Node::SharedPtr node) 
    {
        /* Common callback for processing 1x1 image */
        auto gs_callback = [this](const sensor_msgs::msg::Image::SharedPtr msg, int index) {
            /* Since the image is 1x1 R8G8B8, msg->data will have 3 bytes (R, G, B) */
            if (!msg->data.empty()) {
                /* Calculate an average brightness value (grayscale) from the R, G and B values */
                int gray = (msg->data[0] + msg->data[1] + msg->data[2]) / 3;
                this->last_grayscale_values[index] = gray;
            }
        };
        

        sub_gs_left = node->create_subscription<sensor_msgs::msg::Image>(
            "/line_follower/left", 10, [this, gs_callback](const sensor_msgs::msg::Image::SharedPtr msg) {
                gs_callback(msg, 0);
            });

        sub_gs_center = node->create_subscription<sensor_msgs::msg::Image>(
            "/line_follower/center", 10, [this, gs_callback](const sensor_msgs::msg::Image::SharedPtr msg) {
                gs_callback(msg, 1);
            });

        sub_gs_right = node->create_subscription<sensor_msgs::msg::Image>(
            "/line_follower/right", 10, [this, gs_callback](const sensor_msgs::msg::Image::SharedPtr msg) {
                gs_callback(msg, 2);
            });
    }


public:
    SimVisionVoyager(rclcpp::Node::SharedPtr node) 
    {
        cmd_pub = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
        pan_pub = node->create_publisher<std_msgs::msg::Float64>("/model/vision_voyager/joint/pan_joint/cmd_pos", 10);
        tilt_pub = node->create_publisher<std_msgs::msg::Float64>("/model/vision_voyager/joint/tilt_joint/cmd_pos", 10);
        setup_ultrasonic_communication(node);
        setup_grayscale_communication(node);
        last_cmd.linear.x = 0.0;
        last_cmd.angular.z = 0.0;
    }

    void publish_all()
    {
        cmd_pub->publish(last_cmd);
        pan_pub->publish(last_pan);
        tilt_pub->publish(last_tilt);
    }

    void set_motor_control(int speed, bool forward) override 
    {
        last_cmd.linear.x = ((double)speed / 10.0) * (forward ? 1.0 : -1.0);
    }

    void set_steering(int angle) override 
    {
        /* Change andgle sign because Gazebo has inverted axis for steering */
        double angle_in_radians = angle * -1.0 * (M_PI / 180.0);
        last_cmd.angular.z = angle_in_radians;
    }

    void set_camera_tilt_angle(int angle) override 
    {
        /* Change andgle sign because Gazebo has inverted axis */
        last_tilt.data = angle * -1.0 * (M_PI / 180.0);     
    }

    void set_cam_pan_angle(int angle) override 
    {
        /* Change andgle sign because Gazebo has inverted axis */
        last_pan.data = angle * -1.0 * (M_PI / 180.0);
    }

    void stop_robot() override 
    {
        last_cmd.linear.x = 0.0;
        last_cmd.angular.z = 0.0;
    }

    vector<int> get_grayscale() override 
    {
        return last_grayscale_values;
    }

    float get_ultrasonic() override 
    {
        return last_ultrasonic_distance;
    }
    
    SevereErrorType run_hall_checks() override 
    {
        // No hall sensors in simulation, so this can be a no-op or return simulated data
        return SevereErrorType::NO_ERROR;
    }
  
};

#endif