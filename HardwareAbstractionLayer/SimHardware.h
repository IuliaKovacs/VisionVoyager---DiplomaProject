#ifndef SIM_HARDWARE_H
#define SIM_HARDWARE_H

#include "HardwareInterfaces.h"
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <std_msgs/msg/float64.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>

#define ULTRASONIC_THRESHOLD_MIN 0.02
#define ULTRASONIC_THRESHOLD_MAX 3.0
#define ULTRASONIC_NO_OBJECT_DISTANCE -1.0

class SimVisionVoyager : public IVisionVoyagerInterface {

private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr pan_pub;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr tilt_pub;
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr ultrasonic_sub;
    geometry_msgs::msg::Twist last_cmd;
    float last_ultrasonic_distance = 0.0;

    void set_ultrasonic_communication(rclcpp::Node::SharedPtr node)
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

public:
    SimVisionVoyager(rclcpp::Node::SharedPtr node) {
        cmd_pub = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
        pan_pub = node->create_publisher<std_msgs::msg::Float64>("/model/vision_voyager/joint/pan_joint/cmd_pos", 10);
        tilt_pub = node->create_publisher<std_msgs::msg::Float64>("/model/vision_voyager/joint/tilt_joint/cmd_pos", 10);
        set_ultrasonic_communication(node);
        last_cmd.linear.x = 0.0;
        last_cmd.angular.z = 0.0;
    }

    void set_motor_control(int speed, bool forward) override {
        last_cmd.linear.x = ((double)speed / 10.0) * (forward ? 1.0 : -1.0);
        cmd_pub->publish(last_cmd);
    }

    void set_steering(int angle) override {
        /* Change andgle sign because Gazebo has inverted axis for steering */
        double angle_in_radians = angle * -1.0 * (M_PI / 180.0);
        last_cmd.angular.z = angle_in_radians;
        cmd_pub->publish(last_cmd);
    }

    void set_camera_tilt_angle(int angle) override {
        auto msg = std_msgs::msg::Float64();
        /* Change andgle sign because Gazebo has inverted axis */
        msg.data = angle * -1.0 * (M_PI / 180.0);     
        tilt_pub->publish(msg);
    }

    void set_cam_pan_angle(int angle) override {
        auto msg = std_msgs::msg::Float64();
        /* Change andgle sign because Gazebo has inverted axis */
        msg.data = angle * -1.0 * (M_PI / 180.0);
        pan_pub->publish(msg);
    }

    void stop_robot() override {
        last_cmd.linear.x = 0.0;
        last_cmd.angular.z = 0.0;
        cmd_pub->publish(last_cmd);
    }

    std::vector<int> get_grayscale() override {
        // Implementation for getting grayscale sensor data in simulation
        return {};
    }

    float get_ultrasonic() override {
        return last_ultrasonic_distance;
    }
    
    SevereErrorType run_hall_checks() override {
        // No hall sensors in simulation, so this can be a no-op or return simulated data
        return SevereErrorType::NO_ERROR;
    }
  
};

#endif