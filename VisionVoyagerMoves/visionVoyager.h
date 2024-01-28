#ifndef VISION_VOYAGER
#define VISION_VOYAGER

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <iostream>

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif


#define DIR_MIN -30
#define DIR_MAX 30
#define DEFAULT_SPEED 10
#define DEFAULT_WHEEL_ANGLE 0
#define DEFAULT_PAN_ANGLE 0
#define DEFAULT_TILT_ANGLE 0
#define CAM_PAN_MIN -90
#define CAM_PAN_MAX 90
#define CAM_TILT_MIN -35
#define CAM_TILT_MAX 65


namespace py = pybind11;

class VisionVoyager
{
    py::object picar_python_module;
    py::object picar_python_class;
    py::object picar_python_object;
    int speed;
    int dir_angle;
    int camera_tilt_angle;
    int camera_pan_angle;
    void initialize_python_embedding();
public:

	VisionVoyager();
    void set_picar_python_object(py::object picar_python_object);
    void set_speed(int new_speed);
    void set_dir_angle(int new_dir_angle);
    void set_camera_tilt_angle(int new_camera_tilt_angle);
    void set_cam_pan_angle(int new_camera_pan_angle);
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
};



#endif  // VISION_VOYAGER