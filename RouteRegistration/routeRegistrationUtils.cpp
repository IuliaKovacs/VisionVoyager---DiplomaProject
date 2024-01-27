#include "routeRegistrationUtils.h"
#include <iostream> 
#include <fstream>
#include <optional>
#include <chrono>
#include <iomanip>
#include <thread>


using namespace std; 


RouteRegistration::RouteRegistration()
{
    this->register_enabled = REGISTRATION_DISABLED;
    moving_state = MovingState::STATIONARY;
}

string RouteRegistration::get_current_timestamp()
{
    auto now = std::chrono::system_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    auto timestamp = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&timestamp), "%Y-%b-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return ss.str();
}

void RouteRegistration::set_moving_state(MovingState moving_state)
{
    this->moving_state = moving_state;
}

bool RouteRegistration::create_route_file(string route_name)
{   
    this->current_route_name = route_name;
    this->route_names.push_back(route_name);
    ofstream Route_File(route_name + ".txt");
    Route_File.close();
    return true;
}


void RouteRegistration::end_registration()
{   
    register_current_move("stop");
    delete_duplicate_line_commands();
    create_record_for_route();
    this->register_enabled = REGISTRATION_DISABLED;
}


void RouteRegistration::register_current_move(string move_name, optional<int> arg_value)
{
    ofstream Route_File;
    Route_File.open(this->current_route_name + ".txt", ios_base::app);

    if (arg_value) 
    {
        Route_File << get_current_timestamp() + " -> " + move_name + "(" << arg_value.value() << ")" << endl;   
    }          
    else
    {
        Route_File << get_current_timestamp() + " -> " + move_name + "(" + ")" << endl;
    }
            
    Route_File.close();
}

void RouteRegistration::register_speed_change(int new_speed)
{
    if (this->moving_state == MovingState::MOVING_FORWARD)
    {
        register_current_move("forward", new_speed);
    }
    else if (this->moving_state == MovingState::REVERSING)
    {
        register_current_move("backward", new_speed);
    }
}

void RouteRegistration::set_register_enabled_true()
{   
    this->register_enabled = REGISTRATION_ENABLED;
    create_route_file("Route_No_1");
}

int RouteRegistration::extract_the_miliseconds_between_two_commands(string command_1, string command_2)
{
    string minutes_1 = command_1.substr(15,2);
    string minutes_2 = command_2.substr(15,2);
    string seconds_1 = command_1.substr(18,2);
    string seconds_2 = command_2.substr(18,2);
    string miliseconds_1 = command_1.substr(21,3);
    string miliseconds_2 = command_2.substr(21,3);
    // cout << "Command 1: M: " << minutes_1 << ", s: " << seconds_1 << ", ms: " << miliseconds_1 << endl;
    // cout << "Command 2: M: " << minutes_2 << ", s: " << seconds_2 << ", ms: " << miliseconds_2 << endl;
    int difference_in_miliseconds = (stoi(minutes_2) - stoi(minutes_1)) * 60 * 1000 + (stoi(seconds_2) - stoi(seconds_1)) * 1000 + (stoi(miliseconds_2) - stoi(miliseconds_1));
    // cout << "Difference in miliseconds between first and second command: " << difference_in_miliseconds << endl;
    return difference_in_miliseconds;
}

void RouteRegistration::create_record_for_route()
{
    string line, prev_line;
    ifstream Route_File("optimized_" + current_route_name + ".txt");
    ofstream Record_File;
    Record_File.open("record_" + current_route_name + ".txt", ios_base::trunc);

    if (Route_File.is_open())
    {
        getline(Route_File,prev_line);

        while (getline(Route_File,line))
        {
            int duration_in_miliseconds = extract_the_miliseconds_between_two_commands(prev_line, line);
            Record_File << prev_line.substr(28) << endl;
            Record_File << duration_in_miliseconds << endl;
            prev_line = line;
        }

        Record_File << prev_line.substr(28) << endl;

        Route_File.close();
    }
}

void RouteRegistration::delete_duplicate_line_commands()
{
    string line, prev_line;
    ifstream Route_File(current_route_name + ".txt");
    ofstream Optimized_File;
    Optimized_File.open("optimized_" + current_route_name + ".txt", ios_base::trunc);

    if (Route_File.is_open())
    {
        getline(Route_File,prev_line);

        Optimized_File << prev_line << endl;

        while (getline(Route_File,line))
        {   
            string prev_cmd = prev_line.substr(28);
            string curr_cmd = line.substr(28);

            if (prev_cmd.compare(curr_cmd) != 0)
            {
                Optimized_File << line << endl;
            }

            prev_line = line;
        }

        Route_File.close();
    }
}

int main()
{
    RouteRegistration routeRegistration = RouteRegistration();

    routeRegistration.set_register_enabled_true();

    routeRegistration.register_current_move("forward", 10);

    routeRegistration.register_current_move("forward", 10);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 3));

    routeRegistration.register_current_move("stop");

    routeRegistration.register_current_move("backward", 10);

    routeRegistration.register_current_move("set_dir_servo_angle", 10);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    routeRegistration.register_current_move("set_cam_pan_angle", 80);

    routeRegistration.register_current_move("set_camera_tilt_angle", 80);

    routeRegistration.delete_duplicate_line_commands();

    routeRegistration.create_record_for_route();


    
    return 0;
}


