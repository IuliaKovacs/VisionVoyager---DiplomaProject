#ifndef VISION_VOYAGER
#define VISION_VOYAGER

#ifndef USE_SIMULATION
    #include <pybind11/embed.h>
    #include <pybind11/pybind11.h>
#else
    #include "rclcpp/rclcpp.hpp"
#endif

#include <iostream>
#include <vector>
#include "../ApplicationModule/application_utils.h"
#include "../HardwareAbstractionLayer/HardwareInterfaces.h"

/* Include both Sim and Real hardware headers, but the compiler will only "see" the relevant code
 based on the USE_SIMULATION flag defined in CMake */
#ifdef USE_SIMULATION
    #include "SimHardware.h"
#else
    #include "RealHardware.h"
#endif

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

#define DIR_MIN_RG -20
#define DIR_MIN_LF -30
#define DIR_MAX_RG 20
#define DIR_MAX_LF 30
#define DEFAULT_SPEED 10
#define DEFAULT_WHEEL_ANGLE 0
#define DEFAULT_PAN_ANGLE 0
#define DEFAULT_TILT_ANGLE 0
#define CAM_PAN_MIN -90
#define CAM_PAN_MAX 90
#define CAM_TILT_MIN -35
#define CAM_TILT_MAX 65

using namespace std;
#ifndef USE_SIMULATION
    namespace py = pybind11;
#endif



class VisionVoyager
{
    IVisionVoyagerInterface* hw;
#ifndef USE_SIMULATION
    py::object picar_python_module;
    py::object picar_python_class;
    py::object picar_python_object;
#endif
    int DIR_MIN;
    int DIR_MAX;
    int speed;
    int dir_angle;
    int camera_tilt_angle;
    int camera_pan_angle;
    float angular_velocity = 0.0f;
    float linear_velocity = 0.0f;
    void initialize_python_embedding();
public:

#ifdef USE_SIMULATION
    /* Constructor used on laptop receives the ROS 2 node */ 
    VisionVoyager(rclcpp::Node::SharedPtr node); 
    void publish_all();
#else
    /* Constructor used on Pi does not receive any parameters */
    VisionVoyager();
    void set_picar_python_object(py::object picar_python_object);
#endif

    void set_speed(int new_speed);
    void set_dir_angle(int new_dir_angle);
    void set_camera_tilt_angle(int new_camera_tilt_angle);
    void set_cam_pan_angle(int new_camera_pan_angle);
    void set_direction_limits(int left_max, int right_max);
    int get_speed();
    int get_dir_angle();
    int get_camera_tilt_angle();
    int get_camera_pan_angle();
    void move_forward();
    void move_backward();
    void turn_left_one_degree();
    void turn_right_one_degree();
    void turn_left_max();
    void turn_right_max();
    void stop();
    void camera_pan_left_one_degree();
    void camera_pan_right_one_degree();
    void camera_pan_left_max();
    void camera_pan_right_max();
    void camera_tilt_up_one_degree();
    void camera_tilt_down_one_degree();
    void camera_tilt_up_max();
    void camera_tilt_down_max();
    void take_photo();
    void speed_up();
    void speed_down();
    vector<int> read_grayscale_data();
    float read_ultrasonic_data();
    SevereErrorType check_hall_sensors_timing();
};



#endif  // VISION_VOYAGER