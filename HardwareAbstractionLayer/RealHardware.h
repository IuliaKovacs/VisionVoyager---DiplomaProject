#ifndef REAL_HARDWARE_H
#define REAL_HARDWARE_H

#include "HardwareInterfaces.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

class RealVisionVoyager : public IVisionVoyagerInterface {

    py::object picar_python_object;

private:
    vector<int> VisionVoyager::map_python_list_cpp_vector(py::list py_list) 
    {
        vector<int> cpp_vector;

        for (const auto& item : py_list) {
            cpp_vector.push_back(py::cast<int>(item));
        }

        return cpp_vector;
    }

public:
    RealVisionVoyager(py::object obj) : picar_python_object(obj) {}

    void set_motor_control(int speed, bool forward) override {
        if(forward) picar_python_object.attr("forward")(speed);
        else picar_python_object.attr("backward")(speed);
    }

    void set_steering_control(int angle) override {
        picar_python_object.attr("set_steering")(angle);
    }

    void set_camera_tilt_angle(int angle) override {
        picar_python_object.attr("set_camera_tilt_angle")(angle);
    }

    void set_cam_pan_angle(int angle) override {
        picar_python_object.attr("set_cam_pan_angle")(angle);
    }

    void stop_robot() override {
        picar_python_object.attr("stop_robot")();
    }

    std::vector<int> get_grayscale() override {
        py::list py_result = this->picar_python_object.attr("get_grayscale_data")();
        py::list py_list = py_result.cast<py::list>();
        vector<int> grayscale_data = map_python_list_cpp_vector(py_list);
        return grayscale_data;
    }

    float get_ultrasonic() override {
        return picar_python_object.attr("get_ultrasonic")().cast<float>();
    }

    SevereErrorType run_hall_checks() override {
        if (wiringPiSetupGpio() == -1) 
        {   // BCM GPIO numbering
            logFile << log_time() << " Error: Failed to initialize WiringPi!" << endl;
            return SevereErrorType::GPIO_ERROR;
        }

        log_mutex.lock();
        logFile << log_time() << LOG_HALL_SENSORS_PREFIX << " Hall Sensors Feature ready for use!" << endl;
        log_mutex.unlock();

        pinMode(HALL_PIN_RIGHT_MOTOR, INPUT);
        pullUpDnControl(HALL_PIN_RIGHT_MOTOR, PUD_UP);
        pinMode(HALL_PIN_LEFT_MOTOR, INPUT);
        pullUpDnControl(HALL_PIN_LEFT_MOTOR, PUD_UP);

        auto last_spin_right = std::chrono::steady_clock::now();
        auto last_spin_left = std::chrono::steady_clock::now();

        vector<long long> last_10_time_intervals_right(NUMBER_OF_PERIODS, 0);
        vector<long long> last_10_time_intervals_left(NUMBER_OF_PERIODS, 0);
        int index_r = -1; //this index will help tracking the latest 10 time intervals/periods
        int index_l = -1; //this index will help tracking the latest 10 time intervals/periods
        bool last_moving_state = false;
        SevereErrorType result = SevereErrorType::NO_ERROR;

        while((!route_complete.load()) && (!severe_error.load()))
        {   
            bool moving_state = moving.load(); 
            if(true == moving_state)
            {   
                /* reset last spin timestamp if the robot is just starting to move again */
                if(false == last_moving_state)
                {
                    last_spin_right = std::chrono::steady_clock::now();
                    last_spin_left = std::chrono::steady_clock::now();
                }

                if(true == last_moving_state)
                {
                    /* checking if it takes too long for a wheel to make a complete turn and abort and issue a warning if so */
                    auto now_time = std::chrono::steady_clock::now();
                    auto duration_right = std::chrono::duration_cast<std::chrono::milliseconds>(now_time - last_spin_right);
                    auto duration_left = std::chrono::duration_cast<std::chrono::milliseconds>(now_time - last_spin_left);
                    if((THRESHOLD_WHEEL_ROTATION_TIME < duration_right.count()) || (THRESHOLD_WHEEL_ROTATION_TIME < duration_left.count()))
                    {   
                        log_mutex.lock();
                        logFile << log_time() << LOG_HALL_SENSORS_PREFIX << " --- SEVERE MOTOR ISSUE, ABORTING... --- !WARNING!" << endl;
                        log_mutex.unlock();
                        lock_guard<mutex> lock2(mtx);
                        severe_error.store(true);
                        waiting_condition.notify_all();
                        return SevereErrorType::MOTOR_ERROR;
                    }
                }

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
                        // logFile << log_time() << LOG_HALL_SENSORS_PREFIX << "[Right Motor] Magnet detected!" << endl;
                        last_spin_right = current_spin_right;
                        /* Taking the period in accound only if the robot is not at the start of the moving, otherwise the calculated period would be eronate */
                        if(true == last_moving_state)
                        {    
                            // logFile << log_time() << LOG_HALL_SENSORS_PREFIX << "[Right Motor] time_difference from last period: " << time_difference.count() << "ms" << endl;
                            
                            /* Checking if the wheel rotation took longer than usual and if so -> LOW VOLTAGE warning */
                            if((time_difference.count() > THRESHOLD_WHEEL_ROTATION_TIME_MIN) && (time_difference.count() < THRESHOLD_WHEEL_ROTATION_TIME))
                            {
                                logFile << log_time() << LOG_HALL_SENSORS_PREFIX << "[Right Motor] WARNING: LOW VOLTAGE DETECTED!" << endl;
                                result = SevereErrorType::LOW_VOLTAGE;
                            }
                            
                            /* Workaround because of the periodic signal that is sent to the GPIO 4 PIN - **reason not known 
                            * Forcing the velocity updates to happen only for periods larger than 500 miliseconds */
                            if (time_difference.count() > 500)
                            {
                                index_r = ((index_r + 1) % last_10_time_intervals_right.size());
                                last_10_time_intervals_right[index_r] = time_difference.count();
                                if(index_r == 9)
                                {
                                    float sum = 0.0f;
                                    for (float period : last_10_time_intervals_right) {
                                        sum += period;
                                    }
                                    log_mutex.lock();
                                    float average_period = sum / last_10_time_intervals_right.size();
                                    // logFile << log_time() << LOG_HALL_SENSORS_PREFIX << "[Right Motor] average period: " << average_period << "ms" << endl;
                                    /* transforming the period in secods from miliseconds */
                                    float average_period_s = average_period / 1000.0;
                                    /* omega = (2*PI)/T  <=> angular speed = (2 * PI)/period */
                                    angular_velocity_r = (2 * CONSTANT_PI) / average_period_s; // rad/s
                                    /* v = omega * r  <=> linear speed = angular velocity * radius */
                                    linear_velocity_r = angular_velocity_r * WHEEL_RADIUS; // m/s
                                    logFile << log_time() << LOG_HALL_SENSORS_PREFIX << "[Right Motor] Angular velocity: " << angular_velocity_r << " rad/s" << endl;
                                    logFile << log_time() << LOG_HALL_SENSORS_PREFIX << "[Right Motor] Linear velocity: " << linear_velocity_r << "m/s" << endl;
                                    log_mutex.unlock();
                                }
                            }
                        }
                    }
                } 

                int state_2 = digitalRead(HALL_PIN_LEFT_MOTOR);
                if (state_2 == LOW) 
                {
                    auto current_spin_left = std::chrono::steady_clock::now();
                    auto time_difference = std::chrono::duration_cast<std::chrono::milliseconds>(current_spin_left - last_spin_left);

                    // logFile << log_time() << LOG_HALL_SENSORS_PREFIX << "[Left Motor] time_difference from last period: " << time_difference.count() << "ms" << endl;

                    /* simulating a kind of debounce */
                    if(time_difference.count() > 100)
                    {   
                        
                        // logFile << log_time() << LOG_HALL_SENSORS_PREFIX << "[Left Motor] Magnet detected!" << endl;
                        last_spin_left = current_spin_left;
                        if(true == last_moving_state)
                        {   
                            /* Checking if the wheel rotation took longer than usual and if so -> LOW VOLTAGE warning */
                            if((time_difference.count() > THRESHOLD_WHEEL_ROTATION_TIME_MIN) && (time_difference.count() < THRESHOLD_WHEEL_ROTATION_TIME))
                            {
                                logFile << log_time() << LOG_HALL_SENSORS_PREFIX << "[Left Motor] WARNING: LOW VOLTAGE DETECTED!" << endl;
                                result = SevereErrorType::LOW_VOLTAGE;
                            }

                            index_l = ((index_l + 1) % last_10_time_intervals_left.size());
                            last_10_time_intervals_left[index_l] = time_difference.count();
                            
                            // logFile << log_time() << LOG_HALL_SENSORS_PREFIX << "[Left Motor] time_difference from last period: " << time_difference.count() << "ms" << endl;
                            
                            if(index_l == 9)
                            {
                                float sum = 0.0f;
                                for (float period : last_10_time_intervals_left) {
                                    sum += period;
                                }

                                log_mutex.lock();
                                float average_period = sum / last_10_time_intervals_left.size();
                                // logFile << log_time() << LOG_HALL_SENSORS_PREFIX << "[Left Motor] average period: " << average_period << "ms" << endl;
                                /* transforming the period in secods from miliseconds */
                                float average_period_s = average_period / 1000.0;
                                /* omega = (2*PI)/T  <=> angular speed = (2 * PI)/period */
                                angular_velocity_l = (2 * CONSTANT_PI) / average_period_s; // rad/s
                                /* v = omega * r  <=> linear speed = angular velocity * radius */
                                linear_velocity_l = angular_velocity_l * WHEEL_RADIUS; // m/s
                                logFile << log_time() << LOG_HALL_SENSORS_PREFIX << "[Left Motor] Angular velocity: " << angular_velocity_l << " rad/s" << endl;
                                logFile << log_time() << LOG_HALL_SENSORS_PREFIX << "[Left Motor] Linear velocity: " << linear_velocity_l << "m/s" << endl;

                                if((angular_velocity_r > 0.0f) && (angular_velocity_l > 0.0f))
                                {
                                    angular_velocity = ((angular_velocity_r) + (angular_velocity_l)) / 2;
                                    linear_velocity = ((linear_velocity_r) + (linear_velocity_l)) / 2;
                                    logFile << log_time() << LOG_HALL_SENSORS_PREFIX << " Angular velocity: " << angular_velocity << " rad/s" << endl;
                                    logFile << log_time() << LOG_HALL_SENSORS_PREFIX << " Linear velocity: " << linear_velocity << "m/s" << endl;   
                                }
                                log_mutex.unlock();
                            }
                        }
                    }
                }
            }

            last_moving_state = moving_state;
        }

        return result;
    }   

};

#endif