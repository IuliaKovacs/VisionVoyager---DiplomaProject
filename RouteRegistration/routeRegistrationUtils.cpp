#include "routeRegistrationUtils.h"
#include <iostream> 
#include <fstream>
#include <optional>
#include <chrono>
#include <iomanip>
#include <thread>
#include <cstdio>

using namespace std; 


bool RouteRegistration::register_enabled = REGISTRATION_DISABLED;
MovingState RouteRegistration::moving_state = MovingState::STATIONARY;
string RouteRegistration::route_database_directory_path = "../RouteDatabase/";
vector<string> RouteRegistration::route_names; 
string RouteRegistration::current_route_name = "";


RouteRegistration::RouteRegistration()
{
    
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
    moving_state = moving_state;
}

bool RouteRegistration::create_route_file(string route_name)
{   
    current_route_name = route_name;
    route_names.push_back(route_name);
    ofstream Route_File(route_database_directory_path + route_name + ".txt");
    Route_File.close();
    return true;
}


void RouteRegistration::end_registration()
{   
    register_current_move("stop");
    delete_duplicate_line_commands();
    create_record_for_route();
    register_enabled = REGISTRATION_DISABLED;
    string auxFilePath = route_database_directory_path + "optimized_" + current_route_name + ".txt";
    remove(auxFilePath.c_str());
}


void RouteRegistration::register_current_move(string move_name, optional<int> arg_value)
{
    ofstream Route_File;
    Route_File.open(route_database_directory_path + current_route_name + ".txt", ios_base::app);

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
    if (moving_state == MovingState::MOVING_FORWARD)
    {
        register_current_move("forward", new_speed);
    }
    else if (moving_state == MovingState::REVERSING)
    {
        register_current_move("backward", new_speed);
    }
}

void RouteRegistration::set_register_enabled_true()
{   
    register_enabled = REGISTRATION_ENABLED;
    string auxFilePath = route_database_directory_path + current_route_name + ".txt";
    remove(auxFilePath.c_str());
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
    ifstream Route_File(route_database_directory_path + "optimized_" + current_route_name + ".txt");
    string record_name = route_database_directory_path + "record_" + current_route_name + ".txt";
    remove(record_name.c_str()); 
    ofstream Record_File;
    Record_File.open(route_database_directory_path + "record_" + current_route_name + ".txt", ios_base::trunc);

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
    ifstream Route_File(route_database_directory_path + current_route_name + ".txt");
    ofstream Optimized_File;
    Optimized_File.open(route_database_directory_path + "optimized_" + current_route_name + ".txt", ios_base::trunc);

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
