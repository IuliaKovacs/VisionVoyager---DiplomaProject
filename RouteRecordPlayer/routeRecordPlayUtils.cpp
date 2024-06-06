#include "routeRecordPlayUtils.h"
#include "../RouteRegistration/routeRegistrationUtils.h"
#include <fstream>
#include <chrono>
#include <ctime>
#include "../LineFollower/lineFollower.h"
#include "../ApplicationModule/application.h"


VisionVoyager* RouteRecordPlayer::robotVisionVoyager = nullptr;
Building_Section RouteRecordPlayer::current_section = Building_Section::UNKNOWN;
bool RouteRecordPlayer::avoiding_activated;

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

void RouteRecordPlayer::play_command(string command_name, int miliseconds, optional<int> command_arg)
{
    if (command_name.compare("set_dir_servo_angle") == 0)
    {   
        if (command_arg)
        {
            robotVisionVoyager->set_dir_angle(command_arg.value());
            if(miliseconds > 2500)
            {
                if(20 <= command_arg.value())
                {   
                    tts_mutex.lock();
                    TextToSpeech::display_turn_right();
                    tts_mutex.unlock();
                }
                else if(-20 >= command_arg.value())
                {
                    tts_mutex.lock();
                    TextToSpeech::display_turn_left();
                    tts_mutex.unlock();
                }
            }
        }
        else
        {
            throw std::runtime_error("There was an error in setting parameters!");
        }
        robotVisionVoyager->move_forward();
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
            tts_mutex.lock();
            TextToSpeech::display_go_forward();
            tts_mutex.unlock();
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
            tts_mutex.lock();
            TextToSpeech::display_go_backward();
            tts_mutex.unlock();
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
        tts_mutex.lock();
        TextToSpeech::display_stop();
        tts_mutex.unlock();
    }

}

void RouteRecordPlayer::play_route(string route_name)
{
    ifstream Route_File(ROUTE_DIR_PATH + route_name + ".txt");
    string line;

    logFile << log_time() << "--- Started playing route \"" << route_name << "\" ---" << endl;

    while (getline(Route_File,line))
    {   
        string command_name = extract_command_name(line);
        string command_arg = extract_command_argument(line);
        
        if (command_arg == ")")
        {
            play_command(command_name, 0);
        }
        else 
        {
            play_command(command_name, 0, stoi(command_arg));
        }

        if (getline(Route_File,line))
        {
            int miliseconds = stoi(line);
            this_thread::sleep_for(std::chrono::milliseconds(miliseconds));   
        }
    }
}




void RouteRecordPlayer::play_route_conditioned(string route_name)
{
    ifstream Route_File(route_name);
    string line;

    log_mutex.lock();
    logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " Route Playing Thread Started" << endl;
    logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " --- Started playing route \"" << route_name << "\" ---" << endl;
    log_mutex.unlock();


    while(!route_complete.load())
    {   
        if(getline(Route_File,line))
        {   
            string command_name = extract_command_name(line);
            string command_arg = extract_command_argument(line);
            
            if (getline(Route_File,line))
            {
                int milliseconds_to_count = stoi(line);

                if (command_arg == ")")
                {
                    play_command(command_name, milliseconds_to_count);
                }
                else 
                {
                    play_command(command_name, milliseconds_to_count, stoi(command_arg));
                }
            
                log_mutex.lock();
                logFile << log_time() << command_name << " : " << milliseconds_to_count << endl;
                log_mutex.unlock(); 


                auto start_time = std::chrono::steady_clock::now();
                auto end_time = start_time + std::chrono::milliseconds(milliseconds_to_count);


                while ((std::chrono::steady_clock::now() < end_time)) 
                {   
                    if(ObstacleAvoidance::check_forward_safety())
                    {
                        if(end_time - std::chrono::steady_clock::now() > std::chrono::seconds(10))
                        {
                            std::chrono::duration<double> time_skipped = ObstacleAvoidance::obstacle_avoid();
                            end_time = end_time + std::chrono::duration_cast<std::chrono::steady_clock::duration>(time_skipped);
                        }
                        else
                        {
                            log_mutex.lock();
                            logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " --- Route Interrupted: Obstacle Detected! -> In this moment of route guiding the obstacle cannot be avoided! ---" << endl;
                            logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " --- Displaying acoustical warning ---" << endl;
                            log_mutex.unlock();
                            severe_error.store(true);
                            error_type = SevereErrorType::IN_AIR;
                            play_command("stop", 0);
                            tts_mutex.lock();
                            if(Language::EN == TextToSpeech::get_language())
                            {   
                                TextToSpeech::display_custom_message("In this moment of route guiding the obstacle cannot be avoided! \n\n\n Aborting the guiding process! \n\n\n Please contact the building staff!");
                            }
                            else
                            {
                                TextToSpeech::display_custom_message("În acest moment al rutei nu se poate ocoli obstacolul! \n\n\n Abandonare proces de ghidare! \n\n\n Contactați personalul clădirii!");
                            }
                            tts_mutex.unlock();
                        }
                    }

                    /* IMPORTANT: The threshold for IN AIR DETECTION must be calibrated before using this feature!
                     * It depends on the surface on which the robot moves! */
                    // if(true == LineFollower::verify_is_in_air())
                    // {
                    //     log_mutex.lock();
                    //     logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " --- Route Interrupted: The robot is either in air or on the edge of something! ---" << endl;
                    //     logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " --- Displaying acoustical warning ---" << endl;
                    //     log_mutex.unlock();
                    //     severe_error.store(true);
                    //     error_type = SevereErrorType::IN_AIR;
                    //     play_command("stop", 0);
                    //     tts_mutex.lock();
                    //     if(Language::EN == TextToSpeech::get_language())
                    //     {   
                    //         TextToSpeech::display_custom_message("The robot is in the air or on the edge of something \n\n\n Aborting the guiding process! \n\n\n Please contact the building staff!");
                    //     }
                    //     else
                    //     {
                    //         TextToSpeech::display_custom_message("Robotul este în aer sau pe marginea unei prăpăstii! \n\n\n Abandonare proces de ghidare \n\n\n Contactați personalul clădirii!");
                    //     }
                    //     tts_mutex.unlock();
                    // }

                    if(severe_error.load())
                    {
                        play_command("stop", 0);  
                        log_mutex.lock();
                        logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " ...Aborting due to severe error..." << endl;
                        log_mutex.unlock();
                        lock_guard<mutex> lock2(mtx);
                        should_stop.store(true);
                        route_complete.store(true);
                        camera_condition.notify_all();
                        cond_v.notify_all();
                        speaking_condition.notify_all();
                        return;
                    }

                    if(should_stop.load())
                    {   
                        play_command("stop", 0);
                        log_mutex.lock();
                        logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " ...Waiting... - Intervention from user - must wait the start signal" << endl;
                        log_mutex.unlock();
                        auto wait_start_time = std::chrono::steady_clock::now();
                        std::unique_lock<std::mutex> lock(mtx);
                        cond_v.wait(lock, []{ return !should_stop.load(); });
                        auto wait_end_time = std::chrono::steady_clock::now();
                        end_time += wait_end_time - wait_start_time;
                        play_command("forward", 0, 1);
                        log_mutex.lock();
                        logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " Waiting Ended " << endl;
                        log_mutex.unlock();
                    }
                }
            }
            else
            {
                if (command_arg == ")")
                {
                    play_command(command_name, 0);
                }
                else 
                {
                    play_command(command_name, 0, stoi(command_arg));
                }
            }
        }
        else
        {
            /* The entire file was parsed so it means that we are at the end of the route -> flags are updated */
            {   
                log_mutex.lock();
                logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " --- Route completed --- " << endl;
                log_mutex.unlock();
                lock_guard<mutex> lock2(mtx);
                should_stop.store(true);
                route_complete.store(true);
                cond_v.notify_all();
                camera_condition.notify_all();
                tts_mutex.lock();
                TextToSpeech::display_destination();
                tts_mutex.unlock();
                speaking_condition.notify_all();
            }
        }
    }
}

SevereErrorType RouteRecordPlayer::check_motors_feedback()
{   
    return robotVisionVoyager->check_hall_sensors_timing();
}

