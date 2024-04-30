#include "routeRecordPlayUtils.h"
#include "../RouteRegistration/routeRegistrationUtils.h"
#include <fstream>
#include <chrono>


VisionVoyager* RouteRecordPlayer::robotVisionVoyager = nullptr;
Building_Section RouteRecordPlayer::current_section = Building_Section::UNKNOWN;

void RouteRecordPlayer::set_robot(VisionVoyager* robot)
{
    RouteRecordPlayer::robotVisionVoyager = robot;
}

void RouteRecordPlayer::set_current_section(Building_Section section)
{
    RouteRecordPlayer::current_section = section;
}

string RouteRecordPlayer::extract_command_name(string command)
{   
    return command.substr(0, command.find('('));
}

string RouteRecordPlayer::extract_command_argument(string command)
{
    return command.substr(command.find('(') + 1, command.find(')'));
}

void RouteRecordPlayer::play_command(string command_name, optional<int> command_arg)
{
    if (command_name.compare("set_dir_servo_angle") == 0)
    {   
        if (command_arg)
        {
            robotVisionVoyager->set_dir_angle(command_arg.value());
        }
        else
        {
            throw std::runtime_error("There was an error in setting parameters!");
        }
    }
    else if (command_name.compare("set_cam_pan_angle") == 0)
    {   
        if (command_arg)
        {
            robotVisionVoyager->set_cam_pan_angle(command_arg.value());
        }
        else
        {
            throw std::runtime_error("There was an error in setting parameters!");
        }
        robotVisionVoyager->move_forward();
    }
    else if (command_name.compare("set_camera_tilt_angle") == 0)
    {   
        if (command_arg)
        {
            robotVisionVoyager->set_camera_tilt_angle(command_arg.value());
        }
        else
        {
            throw std::runtime_error("There was an error in setting parameters!");
        }
        robotVisionVoyager->move_forward();
    }
    else if (command_name.compare("forward") == 0)
    {   
        if (command_arg)
        {
            robotVisionVoyager->set_speed(command_arg.value());
        }
        else
        {
            throw std::runtime_error("There was an error in setting parameters!");
        }
        robotVisionVoyager->move_forward();
    }
    else if (command_name.compare("backward") == 0)
    {   
        if (command_arg)
        {
            robotVisionVoyager->set_speed(command_arg.value());
        }
        else
        {
            throw std::runtime_error("There was an error in setting parameters!");
        }
        robotVisionVoyager->move_backward();
    }
    else if (command_name.compare("stop") == 0)
    {
        robotVisionVoyager->stop();
    }

}

void RouteRecordPlayer::play_route(string route_name)
{
    ifstream Route_File(ROUTE_DIR_PATH + route_name + ".txt");
    string line;

    while (getline(Route_File,line))
    {   
        string command_name = extract_command_name(line);
        string command_arg = extract_command_argument(line);
        
        if (command_arg == ")")
        {
            play_command(command_name);
        }
        else 
        {
            play_command(command_name, stoi(command_arg));
        }

        if (getline(Route_File,line))
        {
            int miliseconds = stoi(line);
            this_thread::sleep_for(std::chrono::milliseconds(miliseconds));   
        }
    }
}



