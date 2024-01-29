#include "visionVoyager.h"

namespace py = pybind11;

using namespace std;

VisionVoyager::VisionVoyager()
{   
    this->speed = DEFAULT_SPEED;
    this->dir_angle = DEFAULT_WHEEL_ANGLE;
    this->camera_tilt_angle = DEFAULT_TILT_ANGLE;
    this->camera_pan_angle = DEFAULT_PAN_ANGLE;
    initialize_python_embedding();
}

void VisionVoyager::set_speed(int new_speed)
{
    this->speed = new_speed;
}

void VisionVoyager::set_dir_angle(int new_dir_angle)
{
    this->dir_angle = new_dir_angle;
    picar_python_object.attr("set_dir_servo_angle")(this->dir_angle);
}

void VisionVoyager::set_camera_tilt_angle(int new_camera_tilt_angle)
{
    this->camera_tilt_angle = new_camera_tilt_angle;
    picar_python_object.attr("set_camera_tilt_angle")(this->camera_tilt_angle);
}

void VisionVoyager::set_cam_pan_angle(int new_camera_pan_angle)
{
    this->camera_pan_angle = new_camera_pan_angle;
    picar_python_object.attr("set_cam_pan_angle")(this->camera_pan_angle);
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

void VisionVoyager::initialize_python_embedding()
{
    this->picar_python_module = py::module::import("picarx");
    this->picar_python_class = picar_python_module.attr("Picarx");
    this->picar_python_object = picar_python_class();
}

void VisionVoyager::move_forward()
{
    this->picar_python_object.attr("forward")(this->speed);
}

void VisionVoyager::move_backward() 
{
    this->picar_python_object.attr("backward")(this->speed);
}

void VisionVoyager::turn_left_one_degree() 
{   
    if (DIR_MIN < this->dir_angle)
    {
        this->dir_angle -= 1;
    }

    this->picar_python_object.attr("set_dir_servo_angle")(dir_angle);
    DEBUG_MSG("Direction angle: " + this->dir_angle);
}

void VisionVoyager::turn_right_one_degree() 
{
    if (DIR_MAX > this->dir_angle)
    {
        this->dir_angle += 1;
    }

    this->picar_python_object.attr("set_dir_servo_angle")(dir_angle);
    DEBUG_MSG("Direction angle: " + this->dir_angle);
}

void VisionVoyager::turn_left_max() 
{   
    this->dir_angle = DIR_MIN;
    this->picar_python_object.attr("set_dir_servo_angle")(dir_angle);
    DEBUG_MSG("Direction angle: " + this->dir_angle);
}


void VisionVoyager::turn_right_max() 
{   
    this->dir_angle = DIR_MAX;
    this->picar_python_object.attr("set_dir_servo_angle")(dir_angle);
    DEBUG_MSG("Direction angle: " + this->dir_angle);
}

void VisionVoyager::stop() 
{
    this->picar_python_object.attr("stop")();
}


void VisionVoyager::set_picar_python_object(py::object picar_python_object)
{
    this->picar_python_object = picar_python_object;
}

void VisionVoyager::camera_pan_left_one_degree()
{
    if (CAM_PAN_MIN < this->camera_pan_angle)
    {
        this->camera_pan_angle -= 1;
    }

    this->picar_python_object.attr("set_cam_pan_angle")(this->camera_pan_angle);
    DEBUG_MSG("Camera pan angle: " + this->camera_pan_angle);
}

void VisionVoyager::camera_pan_right_one_degree()
{
    if (CAM_PAN_MAX > this->camera_pan_angle)
    {
        this->camera_pan_angle += 1;
    }

    this->picar_python_object.attr("set_cam_pan_angle")(this->camera_pan_angle);
    DEBUG_MSG("Camera pan angle: " + this->camera_pan_angle);
}

void VisionVoyager::camera_pan_left_max()
{
    this->camera_pan_angle = CAM_PAN_MIN;
    this->picar_python_object.attr("set_cam_pan_angle")(this->camera_pan_angle);
    DEBUG_MSG("Camera pan angle: " + this->camera_pan_angle);
}

void VisionVoyager::camera_pan_right_max()
{
    this->camera_pan_angle = CAM_PAN_MAX;
    this->picar_python_object.attr("set_cam_pan_angle")(this->camera_pan_angle);
    DEBUG_MSG("Camera pan angle: " + this->camera_pan_angle);
}

void VisionVoyager::camera_tilt_up_one_degree()
{
    if (CAM_TILT_MIN < this->camera_tilt_angle)
    {
        this->camera_tilt_angle += 1;
    }

    this->picar_python_object.attr("set_camera_tilt_angle")(this->camera_tilt_angle);
    DEBUG_MSG("Camera tilt angle: " + this->camera_tilt_angle);
}

void VisionVoyager::camera_tilt_down_one_degree()
{
    if (CAM_TILT_MAX > this->camera_tilt_angle)
    {
        this->camera_tilt_angle -= 1;
    }

    this->picar_python_object.attr("set_camera_tilt_angle")(this->camera_tilt_angle);
    DEBUG_MSG("Camera tilt angle: " + this->camera_tilt_angle);
}

void VisionVoyager::camera_tilt_up_max()
{   
    this->camera_tilt_angle = CAM_TILT_MIN;
    this->picar_python_object.attr("set_camera_tilt_angle")(this->camera_tilt_angle);
    DEBUG_MSG("Camera tilt angle: " + this->camera_tilt_angle);
}

void VisionVoyager::camera_tilt_down_max()
{
    this->camera_tilt_angle = CAM_TILT_MAX;
    this->picar_python_object.attr("set_camera_tilt_angle")(this->camera_tilt_angle);
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

