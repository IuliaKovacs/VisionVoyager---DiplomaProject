#ifndef SIM_HARDWARE_H
#define SIM_HARDWARE_H

#include "HardwareInterfaces.h"
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>

class SimVisionVoyager : public IVisionVoyagerInterface {
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub;
    geometry_msgs::msg::Twist last_cmd;

public:
    SimVisionVoyager(rclcpp::Node::SharedPtr node) {
        cmd_pub = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
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
        // Implementation for setting camera tilt angle in simulation
    }

    void set_cam_pan_angle(int angle) override {
        // Implementation for setting camera servos in simulation
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
        // Implementation for getting ultrasonic sensor data in simulation
        return 0.0;
    }
    
    SevereErrorType run_hall_checks() override {
        // No hall sensors in simulation, so this can be a no-op or return simulated data
        return SevereErrorType::NO_ERROR;
    }
  
};

#endif