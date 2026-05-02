#ifndef SIM_HARDWARE_H
#define SIM_HARDWARE_H

#include "HardwareInterfaces.h"
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>

class SimVisionVoyager : public IVisionVoyagerInterface {
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub;

public:
    SimVisionVoyager(rclcpp::Node::SharedPtr node) {
        cmd_pub = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
    }

    void set_motor_control(int speed, bool forward) override {
        auto msg = geometry_msgs::msg::Twist();
        msg.linear.x = forward ? 0.5 : -0.5;
        cmd_pub->publish(msg);
    }

    /* @ToDo */
    void set_steering(int angle) override {
        // Implementation for setting steering in simulation
    }

    void set_camera_tilt_angle(int angle) override {
        // Implementation for setting camera tilt angle in simulation
    }

    void set_cam_pan_angle(int angle) override {
        // Implementation for setting camera servos in simulation
    }

    void stop_robot() override {
        // Implementation for stopping robot in simulation
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