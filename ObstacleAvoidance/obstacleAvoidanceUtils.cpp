

#include "obstacleAvoidanceUtils.h"
#include "../RouteRecordPlayer/routeRecordPlayUtils.h"
#include "../TextToSpeech/textToSpeech.h"


using namespace std;

VisionVoyager* ObstacleAvoidance::robotVisionVoyager = nullptr;
float ObstacleAvoidance::ultrasonic_data;
 std::chrono::time_point<std::chrono::steady_clock> ObstacleAvoidance::start_tts_avoid;


void ObstacleAvoidance::set_robot(VisionVoyager* robot)
{
    ObstacleAvoidance::robotVisionVoyager = robot;
}

void ObstacleAvoidance::get_ultrasonic_data()
{
    ObstacleAvoidance::ultrasonic_data = robotVisionVoyager->read_ultrasonic_data();
    // logFile << log_time() << "Ultrasonic: " << ultrasonic_data << endl;
}

Direction ObstacleAvoidance::choose_avoiding_side()
{
    return Direction::RIGHT;
}

bool ObstacleAvoidance::check_forward_safety()
{
    static int counter = 0;

    get_ultrasonic_data();
    if(DANGER_DISTANCE_THRESHOLD > ultrasonic_data)
    {
        counter++;
    }
    else
    {
        counter = 0;
    }

    if((DANGER_DISTANCE_THRESHOLD > ultrasonic_data) && (-1 != ultrasonic_data) && (counter >= 3))
    {
        robotVisionVoyager->stop();
        start_tts_avoid = std::chrono::steady_clock::now();
        log_mutex.lock();
        logFile << log_time() << "[Obstacle Avoiding] Obstacle Detected!" << endl;
        log_mutex.unlock();
        tts_mutex.lock();
        if(Language::EN == TextToSpeech::get_language())
        {   
            TextToSpeech::display_custom_message("Stop! Be careful! Obstacle Detected!");
        }
        else
        {
            TextToSpeech::display_custom_message("Stop! Atenție! Obstacol Detectat!");
        }
        tts_mutex.unlock();
        return true;
    }
    else
    {
        return false;
    }
}


bool ObstacleAvoidance::check_severe_danger()
{
    static int counter = 0;

    get_ultrasonic_data();
    if(SEVERE_DISTANCE_THRESHOLD > ultrasonic_data)
    {
        counter++;
    }
    else
    {
        counter = 0;
    }

    if((DANGER_DISTANCE_THRESHOLD > ultrasonic_data) && (-1 != ultrasonic_data) && (counter >= 3))
    {
        robotVisionVoyager->stop();
        start_tts_avoid = std::chrono::steady_clock::now();
        log_mutex.lock();
        logFile << log_time() << "[Obstacle Avoiding] Severe Danger -> Obstacle Detected!" << endl;
        log_mutex.unlock();
        tts_mutex.lock();
        if(Language::EN == TextToSpeech::get_language())
        {   
            TextToSpeech::display_custom_message("Stop! Be careful! A new obstacle is detected!");
        }
        else
        {
            TextToSpeech::display_custom_message("Stop! Atenție! Un nou obstacol detectat!");
        }
        tts_mutex.unlock();
        return true;
    }
    return false;
}


std::chrono::duration<double> ObstacleAvoidance::obstacle_avoid()
{
    
    std::chrono::duration<double> time_skipped = std::chrono::seconds(0);
    Direction side = Direction::RIGHT;
    if(Language::EN == TextToSpeech::get_language())
    {   
        TextToSpeech::display_custom_message("The obstacle should be avoided on right side! Take a step to the right, two steps forward, and one step to the left");
    }
    else
    {
        TextToSpeech::display_custom_message("Obstacolul ar trebui ocolit pe partea dreapta! Faceți un pas în dreapta, mergeți înainte 2 pași, iar apoi faceți un pas în stânga");
    }
    auto end_tts = std::chrono::steady_clock::now();

    auto start_time = std::chrono::steady_clock::now();
    log_mutex.lock();
    logFile << log_time() << "[Obstacle Avoiding] Start Avoiding" << endl;
    log_mutex.unlock();
    if (side == Direction::RIGHT)
    {
        log_mutex.lock();
        logFile << log_time() << "[Obstacle Avoiding] Avoiding an obstacle - choosed right side to avoid" << endl;
        log_mutex.unlock();
        // avoid_simple_obstacle_right_side(); @ToDo
        RouteRecordPlayer::avoid_right();
    }
    else if (side == Direction::LEFT)
    {
        log_mutex.lock();
        logFile << log_time() << "[Obstacle Avoiding] Avoiding an obstacle - choosed left side to avoid" << endl;
        log_mutex.unlock();
        // avoid_simple_obstacle_left_side(); @ToDo
    }
    log_mutex.lock();
    logFile << log_time() << "[Obstacle Avoiding] Obstacle Avoided Successfully!" << endl;
    log_mutex.unlock();
    auto end_time = std::chrono::steady_clock::now(); 
    time_skipped = (end_tts - start_tts_avoid) + (end_time - start_time); //'(' ')' for better visibility in code
    logFile << log_time() << "[Obstacle Avoiding] Time Skipped: " << time_skipped.count() << endl;
    return time_skipped;
}




void ObstacleAvoidance::return_on_track()
{
    vector<string> lines; 
    
    ifstream AvoidanceAux("../ObstacleAvoidance/obstacleAvoidanceAux.txt");

    if (AvoidanceAux.is_open())
    {
        string line;

        while(getline(AvoidanceAux, line))
        {
            lines.push_back(line);
        }

        AvoidanceAux.close();
    }
    else
    {
        cerr << "Unable to open file." << endl;
    }

    int index = 0;

    while(index < lines.size())
    {
        
        string command_name = RouteRecordPlayer::extract_command_name(lines[index]);
        string command_arg_aux = RouteRecordPlayer::extract_command_argument(lines[index]);
        index++;
        int milliseconds = stoi(lines[index]);
        index++;
        int command_arg;

        if (!(command_arg_aux == ")"))
        {
            command_arg = stoi(command_arg_aux);

            // @To be replaced with new commands
            // if (command_arg == DIR_MIN)
            // {
            //     command_arg = DIR_MAX;
            // }
            // else if (command_arg == DIR_MAX)
            // {
            //     command_arg = DIR_MIN;
            // }
        }
        else
        {
            /* raise Error */
        }

        RouteRecordPlayer::play_command(command_name, milliseconds, command_arg);
        // logFile << log_time() << command_name << "(" << command_arg << ")" << endl;
        this_thread::sleep_for(std::chrono::milliseconds(milliseconds)); 
    }

    robotVisionVoyager->stop();
    

}


void ObstacleAvoidance::reverse_route(string route_name)
{
    vector<string> lines; 
    
    ifstream AvoidanceAux("../ObstacleAvoidance/obstacleAvoidanceAux.txt");

    if (AvoidanceAux.is_open())
    {
        string line;

        while(getline(AvoidanceAux, line))
        {
            lines.push_back(line);
        }

        AvoidanceAux.close();
    }
    else
    {
        cerr << "Unable to open file." << endl;
    }
    
    robotVisionVoyager->move_backward();

    while(!lines.empty())
    {      
        int milliseconds = stoi(lines.back());
        lines.pop_back();
        string command_name = RouteRecordPlayer::extract_command_name(lines.back());
        string command_arg_aux = RouteRecordPlayer::extract_command_argument(lines.back());
        lines.pop_back();
        int command_arg;

        if (!(command_arg_aux == ")"))
        {
            command_arg = stoi(command_arg_aux);
        }

        if (command_name == "forward")
        {
            command_name == "backward";
        }

        RouteRecordPlayer::play_command(command_name, milliseconds, command_arg);
        this_thread::sleep_for(std::chrono::milliseconds(milliseconds)); 
    }

    robotVisionVoyager->stop();
}


