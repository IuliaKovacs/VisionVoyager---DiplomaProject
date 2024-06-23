#include "routeRecordPlayUtils.h"
#include "../RouteRegistration/routeRegistrationUtils.h"
#include <fstream>
#include <chrono>
#include <ctime>
#include "../LineFollower/lineFollower.h"
#include "../ApplicationModule/application.h"
#include <stdio.h>



#define OBSTACLE_RIGHT_AVOID_FILE "../RouteDatabase/avoid_obstacle.txt"


typedef enum{
    RFID_SEND_PING_EN,
    RFID_START_SYSTEM_INIT_EN,
    RFID_STATUS_SYSTEM_INIT_EN,
    RFID_START_ROOM_SEARCH_EN,
    RFID_STATUS_ROOM_SEARCH_EN,
    RFID_START_GET_ROOMS_EN,
    RFID_STATUS_GET_ROOMS_EN
}RFID_Subsystem_request_en;

static RFID_Subsystem_request_en request_state_en = RFID_SEND_PING_EN;
uint8_t out_data;
RFID_request_status_t st;
RFID_Tag_Information tag_info;
bool op_status;
bool system_init_status;



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
            if(miliseconds > 1200 && (!avoiding.load()))
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
        // robotVisionVoyager->move_forward();
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
        // robotVisionVoyager->move_forward();
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
        // robotVisionVoyager->move_forward();
    }
    else if (command_name.compare("forward") == 0)
    {   
        if (command_arg)
        {
            robotVisionVoyager->set_speed(command_arg.value());
            if(!avoiding.load())
            {
                tts_mutex.lock();
                TextToSpeech::display_go_forward();
                tts_mutex.unlock();
            }
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
            if(!avoiding.load())
            {
                tts_mutex.lock();
                TextToSpeech::display_go_backward();
                tts_mutex.unlock();
            }
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
    char last_room_name[20] = "";
    strcpy(tag_info.room_name, ""); 

    RFID_init();
    // st = RFID_Send_Ping();

    // if(st == RFID_REQUEST_OK)
    // {
    //     logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " [RFID] Ping result: RFID_REQUEST_OK " << endl;
    //     st = RFID_start_get_System_Init_Status(&system_init_status);
    // }
    // else
    // {
    //     // @ToDo TTS message
    // }

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
                    RFID_helper(&tag_info);

                    if(strcmp(tag_info.room_name, "") != 0)
                    {
                        if(strcmp(last_room_name, tag_info.room_name) != 0)
                        {   
                            string room_description_message;
                            logFile << log_time() << LOG_RFID_PREFIX << " Room name: " << tag_info.room_name << endl;
                            logFile << log_time() << LOG_RFID_PREFIX << " Room description: " << tag_info.room_description << endl;
                            logFile << log_time() << LOG_RFID_PREFIX << " Room destination_status: " << tag_info.destination_node << endl;
                            if(Language::EN == TextToSpeech::get_language())
                            {
                                if(false == tag_info.destination_node)
                                {
                                    room_description_message += "We pass by " + string(tag_info.room_name) + "\n\n\n\n\n\n";
                                    room_description_message += "Use the waiting command to stop here! \n\n\n\n\n\n";
                                }
                                else
                                {   
                                    room_description_message += "We reached " + string(tag_info.room_name) + "\n\n\n\n\n\n";
                                    room_description_message += "This is your destination! \n\n\n\n\n\n";
                                }
                                room_description_message += string(tag_info.room_description) + "\n\n\n\n\n\n";
                            }
                            else
                            {
                                if(false == tag_info.destination_node)
                                {
                                    room_description_message += "Trecem pe lângă " + string(tag_info.room_name) + "\n\n\n\n\n\n";
                                    room_description_message += "Folosește comanda de așteptare dacă dorești să ne oprim aici! \n\n\n\n\n\n";
                                }
                                else
                                {   
                                    room_description_message += "Am ajuns la: " + string(tag_info.room_name) + "\n\n\n\n\n\n";
                                    room_description_message += "Aceasta este destinația ta! \n\n\n\n\n\n";
                                }
                                room_description_message += string(tag_info.room_description) + "\n\n\n\n\n\n";
                            }
                            strcpy(last_room_name, tag_info.room_name);
                            /* Send data for displaying the tag info */
                            std::lock_guard<std::mutex> lock(speak_mutex);
                            global_message = room_description_message;
                            strcpy(tag_info.room_name, "");
                            speak.store(true);
                            speaking_condition.notify_all();
                            string tag_info_route_name = string(tag_info.room_name);
                            if((tag_info_route_name == RouteRegistration::get_route_name_from_path(route_name)) && (true == tag_info.destination_node))
                            {
                                route_complete.store(true);
                                logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " [RFID] Destination Reached Successfully - check done by RFID Reader" << endl;
                            }            
                        }
                    }

                    // if(ObstacleAvoidance::check_forward_safety())
                    // {
                    //     if((end_time - std::chrono::steady_clock::now()) > std::chrono::seconds(7))
                    //     {
                    //         std::chrono::duration<double> time_skipped = ObstacleAvoidance::obstacle_avoid();
                    //         robotVisionVoyager->move_forward();
                    //         end_time = end_time + std::chrono::duration_cast<std::chrono::steady_clock::duration>(time_skipped);
                    //     }
                    //     else
                    //     {
                    //         log_mutex.lock();
                    //         logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " --- Route Interrupted: Obstacle Detected! -> In this moment of route guiding the obstacle cannot be avoided! ---" << endl;
                    //         logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " --- Displaying acoustical warning ---" << endl;
                    //         log_mutex.unlock();
                    //         severe_error.store(true);
                    //         error_type = SevereErrorType::IN_AIR;
                    //         play_command("stop", 0);
                    //         tts_mutex.lock();
                    //         if(Language::EN == TextToSpeech::get_language())
                    //         {   
                    //             TextToSpeech::display_custom_message("In this moment of route guiding the obstacle cannot be avoided! \n\n\n Aborting the guiding process! \n\n\n Please contact the building staff!");
                    //         }
                    //         else
                    //         {
                    //             TextToSpeech::display_custom_message("În acest moment al rutei nu se poate ocoli obstacolul! \n\n\n Abandonare proces de ghidare! \n\n\n Contactați personalul clădirii!");
                    //         }
                    //         tts_mutex.unlock();
                    //     }
                    // }

                    // /* IMPORTANT: The threshold for IN AIR DETECTION must be calibrated before using this feature!
                    //  * It depends on the surface on which the robot moves! */
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
                        waiting_condition.notify_all();
                        speaking_condition.notify_all();
                        return;
                    }

                    if(should_stop.load())
                    {   
                        auto wait_start_time = std::chrono::steady_clock::now();
                        play_command("stop", 0);
                        log_mutex.lock();
                        logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " ...Waiting... - Intervention from user - must wait the start signal" << endl;
                        log_mutex.unlock();
                        std::unique_lock<std::mutex> lock(mtx);
                        waiting_condition.wait(lock, []{ return !should_stop.load(); });
                        auto wait_end_time = std::chrono::steady_clock::now();
                        end_time = end_time + (wait_end_time - wait_start_time);
                        // log_mutex.lock();
                        // logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << "New End Time: " << end_time << endl;
                        // log_mutex.unlock();
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
                waiting_condition.notify_all();
                camera_condition.notify_all();
                speaking_condition.notify_all();
                tts_mutex.lock();
                TextToSpeech::display_destination();
                tts_mutex.unlock();
            }
        }
    }
}

SevereErrorType RouteRecordPlayer::check_motors_feedback()
{   
    return robotVisionVoyager->check_hall_sensors_timing();
}


void RouteRecordPlayer::avoid_right()
{
    avoiding.store(true);
    ifstream Route_File(OBSTACLE_RIGHT_AVOID_FILE);
    string line;

    while(getline(Route_File,line))
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
                if(ObstacleAvoidance::check_severe_danger())
                {
                    log_mutex.lock();
                    logFile << log_time() << "[Obstacle Avoiding]" << " --- Route Interrupted: Obstacle Detected! -> In this moment of route guiding the obstacle cannot be avoided! ---" << endl;
                    log_mutex.unlock();
                    severe_error.store(true);
                    error_type = SevereErrorType::ROUTE_ERROR;
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

                /* IMPORTANT: The threshold for IN AIR DETECTION must be calibrated before using this feature!
                    * It depends on the surface on which the robot moves! */
                if(true == LineFollower::verify_is_in_air())
                {
                    log_mutex.lock();
                    logFile << log_time() << "[Obstacle Avoidance]" << " --- Route Interrupted: The robot is either in air or on the edge of something! ---" << endl;
                    logFile << log_time() << "[Obstacle Avoidance]" << " --- Displaying acoustical warning ---" << endl;
                    log_mutex.unlock();
                    severe_error.store(true);
                    error_type = SevereErrorType::IN_AIR;
                    play_command("stop", 0);
                    tts_mutex.lock();
                    if(Language::EN == TextToSpeech::get_language())
                    {   
                        TextToSpeech::display_custom_message("The robot is in the air or on the edge of something \n\n\n Aborting the guiding process! \n\n\n Please contact the building staff!");
                    }
                    else
                    {
                        TextToSpeech::display_custom_message("Robotul este în aer sau pe marginea unei prăpăstii! \n\n\n Abandonare proces de ghidare \n\n\n Contactați personalul clădirii!");
                    }
                    tts_mutex.unlock();
                }

                if(severe_error.load())
                {
                    play_command("stop", 0);  
                    log_mutex.lock();
                    logFile << log_time() << "[Obstacle Avoidance]" << " ...Aborting due to severe error..." << endl;
                    log_mutex.unlock();
                    lock_guard<mutex> lock2(mtx);
                    should_stop.store(true);
                    route_complete.store(true);
                    camera_condition.notify_all();
                    waiting_condition.notify_all();
                    speaking_condition.notify_all();
                    avoiding.store(false);
                    return;
                }

                if(should_stop.load())
                {   
                    auto wait_start_time = std::chrono::steady_clock::now();
                    play_command("stop", 0);
                    log_mutex.lock();
                    logFile << log_time() << "[Obstacle Avoidance]" << " ...Waiting... - Intervention from user - must wait the start signal" << endl;
                    log_mutex.unlock();
                    std::unique_lock<std::mutex> lock(mtx);
                    waiting_condition.wait(lock, []{ return !should_stop.load(); });
                    auto wait_end_time = std::chrono::steady_clock::now();
                    end_time = end_time + (wait_end_time - wait_start_time);
                    play_command("forward", 0, 1);
                    log_mutex.lock();
                    logFile << log_time() << "[Obstacle Avoidance]" << " Waiting Ended " << endl;
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
    avoiding.store(false);
}


bool RouteRecordPlayer::RFID_helper(RFID_Tag_Information *out_tag_info)
{
    static bool result = false;

    switch(request_state_en)
    {
        case RFID_SEND_PING_EN:
            st = RFID_Send_Ping();
            if(st == RFID_REQUEST_OK)
            {
                request_state_en = RFID_START_SYSTEM_INIT_EN;
            }
            else
            {
                request_state_en = RFID_SEND_PING_EN;
            }

        break;

        case RFID_START_SYSTEM_INIT_EN:
        st = RFID_start_get_System_Init_Status(&system_init_status);
        if(st == RFID_REQUEST_PENDING)
        {
            request_state_en = RFID_STATUS_SYSTEM_INIT_EN;
        }
        else if((st == RFID_REQUEST_OK) && (system_init_status == true))
        {
            request_state_en = RFID_START_ROOM_SEARCH_EN;
        }
        break;

        case RFID_STATUS_SYSTEM_INIT_EN:
        st = RFID_status_get_System_Init_Status(&system_init_status);
        if((st == RFID_REQUEST_OK) && (system_init_status == true))
        {
            request_state_en = RFID_START_ROOM_SEARCH_EN;
        }
        else if(st == RFID_REQUEST_PENDING)
        {
            request_state_en = RFID_STATUS_SYSTEM_INIT_EN;
        }
        else 
        {
            cout << "RFID System not initalized\n";
            request_state_en = RFID_SEND_PING_EN;
        }
        break;

        case RFID_START_ROOM_SEARCH_EN:
        st = RFID_start_Room_Search(&op_status);
        result = false;

        if(st == RFID_REQUEST_PENDING)
        {
            request_state_en = RFID_STATUS_ROOM_SEARCH_EN;
        }
        else if(st == RFID_REQUEST_OK && op_status == true)
        {
            request_state_en = RFID_START_GET_ROOMS_EN;
        }

        break;

        case RFID_STATUS_ROOM_SEARCH_EN:
        st = RFID_status_Room_Search(&op_status);
        // cout << "st: " << st << endl;
        // cout << "op_status: " << op_status << endl;
        if(st == RFID_REQUEST_PENDING)
        {
            request_state_en = RFID_STATUS_ROOM_SEARCH_EN;
        }
        else if(st == RFID_REQUEST_OK && op_status == true)
        {
            request_state_en = RFID_START_GET_ROOMS_EN;
        }
        else if(st == RFID_REQUEST_OK && op_status == false)
        {
            request_state_en = RFID_START_ROOM_SEARCH_EN;
        }
        break;

        case RFID_START_GET_ROOMS_EN:
        st=RFID_start_get_Rooms(&op_status);
        if(st == RFID_REQUEST_PENDING)
        {
            request_state_en = RFID_STATUS_GET_ROOMS_EN;
        }
        else if(st == RFID_REQUEST_OK && op_status == true)
        {
            request_state_en = RFID_STATUS_GET_ROOMS_EN;
        }
        break;

        case RFID_STATUS_GET_ROOMS_EN:
        st = RFID_status_get_Rooms(out_tag_info);
        if(st == RFID_REQUEST_OK || st == RFID_REQUEST_NO_TAGS)
        {
            // static auto start_rfid = std::chrono::steady_clock::now();
            // static chrono::time_point<std::chrono::steady_clock> end_rfid_time = start_rfid + std::chrono::milliseconds(1000);
            
            /*INSEAMNA CA E OK*/
            // if(std::chrono::steady_clock::now() > end_rfid_time)
            // {
                request_state_en = RFID_START_ROOM_SEARCH_EN;
                result = true;
            //     start_rfid = std::chrono::steady_clock::now();
            //     end_rfid_time = start_rfid + std::chrono::milliseconds(1000);
            // }
        }
        break;

        default:
        break;
    }
    cout << request_state_en << endl;
    return result;
}
