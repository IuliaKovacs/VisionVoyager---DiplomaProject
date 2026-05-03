#include "visionVoyager.h"
#ifndef USE_SIMULATION
    #include <wiringPi.h>   
#endif

#define HALL_PIN_RIGHT_MOTOR 4 // in BCM numbering
#define HALL_PIN_LEFT_MOTOR 17
#define WHEEL_RADIUS 0.03175 // m - meters = 3.175 cm
#define CONSTANT_PI 3.14159265358979323846
#define WHEEL_CIRCUMFERENCE (2 * CONSTANT_PI * WHEEL_RADIUS) // m - meters

#define THRESHOLD_WHEEL_ROTATION_TIME 3000 // ms - miliseconds 
#define THRESHOLD_WHEEL_ROTATION_TIME_MIN 2000
#define NUMBER_OF_PERIODS 10

#ifndef USE_SIMULATION
    namespace py = pybind11;
#endif
using namespace std;



#ifdef USE_SIMULATION
/* Constructor used on laptop receives the ROS 2 node */ 
VisionVoyager::VisionVoyager(rclcpp::Node::SharedPtr node) 
#else
/* Constructor used on Pi does not receive any parameters */
VisionVoyager::VisionVoyager() 
#endif
{    
    this->speed = DEFAULT_SPEED;
    this->dir_angle = DEFAULT_WHEEL_ANGLE;
    this->camera_tilt_angle = DEFAULT_TILT_ANGLE;
    this->camera_pan_angle = DEFAULT_PAN_ANGLE;

#ifdef USE_SIMULATION
    /* Send the node to the simulation hardware in order to create Publishers */
    this->hw = new SimVisionVoyager(node); 
    DEBUG_MSG("Compiled for SIMULATION mode");
#else
    /* Original logic for Raspberry Pi */
    initialize_python_embedding();
    this->hw = new RealVoyager(this->picar_python_object);
    DEBUG_MSG("Compiled for REAL hardware mode");
#endif
}

void VisionVoyager::set_speed(int new_speed)
{
    this->speed = new_speed;
}

void VisionVoyager::set_dir_angle(int new_dir_angle)
{
    this->dir_angle = new_dir_angle;
    hw->set_steering(this->dir_angle);
}

void VisionVoyager::set_camera_tilt_angle(int new_camera_tilt_angle)
{
    this->camera_tilt_angle = new_camera_tilt_angle;
    hw->set_camera_tilt_angle(this->camera_tilt_angle);
}

void VisionVoyager::set_cam_pan_angle(int new_camera_pan_angle)
{
    this->camera_pan_angle = new_camera_pan_angle;
    hw->set_cam_pan_angle(this->camera_pan_angle);
}

int VisionVoyager::get_speed()
{
    return this->speed;
}

int VisionVoyager::get_dir_angle()
{
    return this->dir_angle;
}

int VisionVoyager::get_camera_pan_angle()
{
    return this->camera_pan_angle;
}

int VisionVoyager::get_camera_tilt_angle()
{
    return this->camera_tilt_angle;
}

#ifndef USE_SIMULATION
void VisionVoyager::initialize_python_embedding()
{
    this->picar_python_module = py::module::import("picarx");
    this->picar_python_class = picar_python_module.attr("Picarx");
    this->picar_python_object = picar_python_class();
}
#endif

void VisionVoyager::move_forward()
{   
    moving.store(true);
    camera_condition.notify_all();
    hw->set_motor_control(this->speed, true);
    logFile << log_time() << "Moving forward" << endl;
}

void VisionVoyager::move_backward() 
{   
    moving.store(true);
    camera_condition.notify_all();
    hw->set_motor_control(this->speed, false);
    logFile << log_time() << "Moving backward" << endl;
}

void VisionVoyager::turn_left_one_degree() 
{   
    if (DIR_MIN < this->dir_angle)
    {
        this->dir_angle -= 1;
    }

    hw->set_steering(this->dir_angle);
    DEBUG_MSG("Direction angle: " + this->dir_angle);
}

void VisionVoyager::turn_right_one_degree() 
{
    if (DIR_MAX > this->dir_angle)
    {
        this->dir_angle += 1;
    }

    hw->set_steering(this->dir_angle);
    DEBUG_MSG("Direction angle: " + this->dir_angle);
}

void VisionVoyager::turn_left_max() 
{   
    this->dir_angle = DIR_MIN;
    hw->set_steering(this->dir_angle);
    DEBUG_MSG("Direction angle: " + this->dir_angle);
}


void VisionVoyager::turn_right_max() 
{   
    this->dir_angle = DIR_MAX;
    hw->set_steering(this->dir_angle);
    DEBUG_MSG("Direction angle: " + this->dir_angle);
}

void VisionVoyager::stop() 
{   
    moving.store(false);
    camera_condition.notify_all();
    hw->stop_robot();
    logFile << log_time() << "Stopped" << endl;
}

#ifndef USE_SIMULATION
void VisionVoyager::set_picar_python_object(py::object picar_python_object)
{
    this->picar_python_object = picar_python_object;
}
#endif

void VisionVoyager::camera_pan_left_one_degree()
{
    if (CAM_PAN_MIN < this->camera_pan_angle)
    {
        this->camera_pan_angle -= 1;
    }

    hw->set_cam_pan_angle(this->camera_pan_angle);
    DEBUG_MSG("Camera pan angle: " + this->camera_pan_angle);
}

void VisionVoyager::camera_pan_right_one_degree()
{
    if (CAM_PAN_MAX > this->camera_pan_angle)
    {
        this->camera_pan_angle += 1;
    }

    hw->set_cam_pan_angle(this->camera_pan_angle);
    DEBUG_MSG("Camera pan angle: " + this->camera_pan_angle);
}

void VisionVoyager::camera_pan_left_max()
{
    this->camera_pan_angle = CAM_PAN_MIN;
    hw->set_cam_pan_angle(this->camera_pan_angle);
    DEBUG_MSG("Camera pan angle: " + this->camera_pan_angle);
}

void VisionVoyager::camera_pan_right_max()
{
    this->camera_pan_angle = CAM_PAN_MAX;
    hw->set_cam_pan_angle(this->camera_pan_angle);
    DEBUG_MSG("Camera pan angle: " + this->camera_pan_angle);
}

void VisionVoyager::camera_tilt_up_one_degree()
{
    if (CAM_TILT_MIN < this->camera_tilt_angle)
    {
        this->camera_tilt_angle += 1;
    }

    hw->set_camera_tilt_angle(this->camera_tilt_angle);
    DEBUG_MSG("Camera tilt angle: " + this->camera_tilt_angle);
}

void VisionVoyager::camera_tilt_down_one_degree()
{
    if (CAM_TILT_MAX > this->camera_tilt_angle)
    {
        this->camera_tilt_angle -= 1;
    }

    hw->set_camera_tilt_angle(this->camera_tilt_angle);
    DEBUG_MSG("Camera tilt angle: " + this->camera_tilt_angle);
}

void VisionVoyager::camera_tilt_up_max()
{   
    this->camera_tilt_angle = CAM_TILT_MIN;
    hw->set_camera_tilt_angle(this->camera_tilt_angle);
    DEBUG_MSG("Camera tilt angle: " + this->camera_tilt_angle);
}

void VisionVoyager::camera_tilt_down_max()
{
    this->camera_tilt_angle = CAM_TILT_MAX;
    hw->set_camera_tilt_angle(this->camera_tilt_angle);
    DEBUG_MSG("Camera tilt angle: " + this->camera_tilt_angle);
}

void VisionVoyager::speed_up()
{
    this->speed += 1;
}

void VisionVoyager::speed_down()
{
    this->speed -= 1;
}

void VisionVoyager::take_photo()
{
    /* @ToDo */
}


vector<int> VisionVoyager::read_grayscale_data()
{
    return hw->get_grayscale();
}

float VisionVoyager::read_ultrasonic_data()
{
    return hw->get_ultrasonic();
}


SevereErrorType VisionVoyager::check_hall_sensors_timing()
{
    return hw->run_hall_checks();
}


void VisionVoyager::set_direction_limits(int left_max, int right_max)
{
    DIR_MIN = left_max;
    DIR_MAX = right_max;
}

#ifdef USE_SIMULATION
void VisionVoyager::publish_all()
{
    hw->publish_all();
}
#endif