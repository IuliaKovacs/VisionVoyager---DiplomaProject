#ifndef HARDWARE_INTERFACES_H
#define HARDWARE_INTERFACES_H

#include <vector>
#include <string>

class IVisionVoyagerInterface {
public:
    virtual ~IVisionVoyagerInterface() = default;

    // Actuators and camera (Output)
    virtual void set_motor_control(int speed, bool forward) = 0;
    virtual void set_steering(int angle) = 0;
    virtual void set_camera_tilt_angle(int angle) = 0;
    virtual void set_cam_pan_angle(int angle) = 0;
    virtual void stop_robot() = 0;

    // Sensors (Input)
    virtual std::vector<int> get_grayscale() = 0;
    virtual float get_ultrasonic() = 0;
    
    // Hall Sensors (Only real HW robot specific)
    virtual SevereErrorType run_hall_checks() = 0;
    
#ifdef USE_SIMULATION
    /* In simulation, needed a method to publish the cmd_vel and servo angles at a regular interval */
    virtual void publish_all() = 0;
#endif

};

#endif