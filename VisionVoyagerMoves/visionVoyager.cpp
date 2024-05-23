#include "visionVoyager.h"
#include <wiringPi.h>


#define HALL_PIN_RIGHT_MOTOR 4 // in BCM numbering
#define HALL_PIN_LEFT_MOTOR 17
#define WHEEL_RADIUS 0.03175 // m - meters = 3.175 cm
#define CONSTANT_PI 3.14159265358979323846
#define WHEEL_CIRCUMFERENCE (2 * CONSTANT_PI * WHEEL_RADIUS) // m - meters

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


vector<int> VisionVoyager::read_grayscale_data()
{
    py::list py_result = this->picar_python_object.attr("get_grayscale_data")();
    py::list py_list = py_result.cast<py::list>();
    vector<int> grayscale_data = map_python_list_cpp_vector(py_list);
    // py::print("Grayscale Data:", result);
    return grayscale_data;
}


vector<int> VisionVoyager::map_python_list_cpp_vector(py::list py_list) 
{
    vector<int> cpp_vector;

    for (const auto& item : py_list) {
        cpp_vector.push_back(py::cast<int>(item));
    }

    return cpp_vector;
}


float VisionVoyager::read_ultrasonic_data()
{
    py::object ultrasonic = this->picar_python_object.attr("ultrasonic");
    float ultrasonic_data = ultrasonic.attr("read")().cast<float>();
    return ultrasonic_data;
}


void VisionVoyager::check_hall_sensors_timing()
{
    if (wiringPiSetupGpio() == -1) 
    {   // BCM GPIO numbering
        logFile << log_time() << " Error: Failed to initialize WiringPi!" << endl;
        return;
    }

    logFile << log_time() << "[Hall Sensor Feature] Ready for use!" << endl;

    pinMode(HALL_PIN_RIGHT_MOTOR, INPUT);
    pullUpDnControl(HALL_PIN_RIGHT_MOTOR, PUD_UP);
    pinMode(HALL_PIN_LEFT_MOTOR, INPUT);
    pullUpDnControl(HALL_PIN_LEFT_MOTOR, PUD_UP);

    auto last_spin_right = std::chrono::steady_clock::now();
    auto last_spin_left = std::chrono::steady_clock::now();

    /* @ToDo replace magic numbers */
    vector<long long> last_10_time_intervals_right(10, 0);
    vector<long long> last_10_time_intervals_left(10, 0);
    int index_r = -1; //this index will help tracking the latest 10 time intervals/periods
    int index_l = -1; //this index will help tracking the latest 10 time intervals/periods

    while (!route_complete.load()) 
    {   
        float angular_velocity_r, angular_velocity_l;
        float linear_velocity_r, linear_velocity_l; 

        int state_1 = digitalRead(HALL_PIN_RIGHT_MOTOR);
        if (state_1 == LOW) 
        {   
            auto current_spin_right = std::chrono::steady_clock::now();
            auto time_difference = std::chrono::duration_cast<std::chrono::milliseconds>(current_spin_right - last_spin_right);
            /* simulating a kind of debounce */
            if(time_difference.count() > 100)
            {
                // logFile << log_time() << "[Hall Sensor][Right Motor] Magnet detected!" << endl;
                last_spin_right = current_spin_right;
                last_10_time_intervals_right[index_r] = time_difference.count();
                // logFile << log_time() << "[Hall Sensor][Right Motor] time_difference from last period: " << time_difference.count() << "ms" << endl;
                /* Workaround because of the periodic signal that is sent to the GPIO 4 PIN - **reason not known 
                * Forcing the velocity updates to happen only for periods larger than 500 miliseconds */
                if (time_difference.count() > 500)
                {
                    index_r = ((index_r + 1) % last_10_time_intervals_right.size());
                    if(index_r == 9)
                    {
                        float sum = 0.0f;
                        for (float period : last_10_time_intervals_right) {
                            sum += period;
                        }
                        float average_period = sum / last_10_time_intervals_right.size();
                        logFile << log_time() << "[Hall Sensor][Right Motor] average period: " << average_period << "ms" << endl;
                        /* transforming the period in secods from miliseconds */
                        float average_period_s = average_period / 1000.0;
                        /* omega = (2*PI)/T  <=> angular speed = (2 * PI)/period */
                        angular_velocity_r = (2 * CONSTANT_PI) / average_period_s; // rad/s
                        /* v = omega * r  <=> linear speed = angular velocity * radius */
                        linear_velocity_r = angular_velocity_r * WHEEL_RADIUS; // m/s
                        logFile << log_time() << "[Hall Sensor][Right Motor] Angular velocity: " << angular_velocity_r << " rad/s" << endl;
                        logFile << log_time() << "[Hall Sensor][Right Motor] Linear velocity: " << linear_velocity_r << "m/s" << endl;
                    }
                }
            }
        } 

        int state_2 = digitalRead(HALL_PIN_LEFT_MOTOR);
        if (state_2 == LOW) 
        {
            auto current_spin_left = std::chrono::steady_clock::now();
            auto time_difference = std::chrono::duration_cast<std::chrono::milliseconds>(current_spin_left - last_spin_left);
            /* simulating a kind of debounce */
            if(time_difference.count() > 100)
            {   
                index_l = ((index_l + 1) % last_10_time_intervals_left.size());
                // logFile << log_time() << "[Hall Sensor][Left Motor] Magnet detected!" << endl;
                last_spin_left = current_spin_left;
                last_10_time_intervals_left[index_l] = time_difference.count();
                // logFile << log_time() << "[Hall Sensor][Left Motor] time_difference from last period: " << time_difference.count() << "ms" << endl;
                if(index_l == 9)
                {
                    float sum = 0.0f;
                    for (float period : last_10_time_intervals_left) {
                        sum += period;
                    }
                    float average_period = sum / last_10_time_intervals_left.size();
                    logFile << log_time() << "[Hall Sensor][Left Motor] average period: " << average_period << "ms" << endl;
                    /* transforming the period in secods from miliseconds */
                    float average_period_s = average_period / 1000.0;
                    /* omega = (2*PI)/T  <=> angular speed = (2 * PI)/period */
                    angular_velocity_l = (2 * CONSTANT_PI) / average_period_s; // rad/s
                    /* v = omega * r  <=> linear speed = angular velocity * radius */
                    linear_velocity_l = angular_velocity_l * WHEEL_RADIUS; // m/s
                    logFile << log_time() << "[Hall Sensor][Left Motor] Angular velocity: " << angular_velocity_l << " rad/s" << endl;
                    logFile << log_time() << "[Hall Sensor][Left Motor] Linear velocity: " << linear_velocity_l << "m/s" << endl;


                    angular_velocity = ((angular_velocity_r) + (angular_velocity_l)) / 2;
                    linear_velocity = ((linear_velocity_r) + (linear_velocity_l)) / 2;
                    logFile << log_time() << "[Hall Sensors] Angular velocity: " << angular_velocity << " rad/s" << endl;
                    logFile << log_time() << "[Hall Sensors] Linear velocity: " << linear_velocity << "m/s" << endl;
                }
            }
        }
    }
}