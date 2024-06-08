

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
    robotVisionVoyager->stop();
    this_thread::sleep_for(std::chrono::milliseconds(1000));
    robotVisionVoyager->set_dir_angle(DIR_MAX);
    robotVisionVoyager->move_forward();
    this_thread::sleep_for(std::chrono::milliseconds(600));
    robotVisionVoyager->stop();
    get_ultrasonic_data();
    logFile << log_time() << ultrasonic_data << endl;
    this_thread::sleep_for(std::chrono::milliseconds(1000));
    robotVisionVoyager->move_backward();
    this_thread::sleep_for(std::chrono::milliseconds(900));
    robotVisionVoyager->stop();
    this_thread::sleep_for(std::chrono::milliseconds(200));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
    

    if ((DANGER_DISTANCE_THRESHOLD < ultrasonic_data) || (-1 == ultrasonic_data))
    {
        return Direction::RIGHT;
    }

    robotVisionVoyager->set_dir_angle(DIR_MIN);
    robotVisionVoyager->move_forward();
    this_thread::sleep_for(std::chrono::milliseconds(600));
    robotVisionVoyager->stop();
    get_ultrasonic_data();
    logFile << log_time() << ultrasonic_data << endl;
    this_thread::sleep_for(std::chrono::milliseconds(1000));
    robotVisionVoyager->move_backward();
    this_thread::sleep_for(std::chrono::milliseconds(900));
    robotVisionVoyager->stop();
    this_thread::sleep_for(std::chrono::milliseconds(200));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);

    if ((DANGER_DISTANCE_THRESHOLD < ultrasonic_data) || (-1 == ultrasonic_data))
    {
        return Direction::LEFT;
    }
}

void ObstacleAvoidance::avoid_simple_obstacle_right_side()
{
    robotVisionVoyager->move_forward();

    robotVisionVoyager->turn_right_max();
    this_thread::sleep_for(std::chrono::milliseconds(1400));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
    this_thread::sleep_for(std::chrono::milliseconds(460));
    robotVisionVoyager->turn_left_max();
    this_thread::sleep_for(std::chrono::milliseconds(900));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
    this_thread::sleep_for(std::chrono::milliseconds(1500));
    robotVisionVoyager->turn_left_max();
    this_thread::sleep_for(std::chrono::milliseconds(875));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
    this_thread::sleep_for(std::chrono::milliseconds(460));
    robotVisionVoyager->turn_right_max();
    this_thread::sleep_for(std::chrono::milliseconds(1100));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
}



void ObstacleAvoidance::avoid_simple_obstacle_left_side()
{
    robotVisionVoyager->move_forward();
   
    robotVisionVoyager->turn_left_max();
    this_thread::sleep_for(std::chrono::milliseconds(1400));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
    this_thread::sleep_for(std::chrono::milliseconds(460));
    robotVisionVoyager->turn_right_max();
    this_thread::sleep_for(std::chrono::milliseconds(900));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
    this_thread::sleep_for(std::chrono::milliseconds(1500));
    robotVisionVoyager->turn_right_max();
    this_thread::sleep_for(std::chrono::milliseconds(875));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
    this_thread::sleep_for(std::chrono::milliseconds(460));
    robotVisionVoyager->turn_left_max();
    this_thread::sleep_for(std::chrono::milliseconds(1100));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE); 
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
        TextToSpeech::display_custom_message("Stop! Atenție! Obstacol Detectat!");
        tts_mutex.unlock();
        return true;
    }
    else
    {
        return false;
    }
}


std::chrono::duration<double> ObstacleAvoidance::obstacle_avoid()
{
    
    std::chrono::duration<double> time_skipped = std::chrono::seconds(0);
    // Direction side = choose_avoiding_side();
    // @ToDo
    Direction side = Direction::RIGHT;
    TextToSpeech::display_custom_message("Obstacolul ar trebui ocolit pe partea dreapta! Faceți doi pași în dreapta, mergeți înainte 2 pași, iar apoi faceți doi pași în stânga");
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
        avoid_simple_obstacle_right_side();
    }
    else if (side == Direction::LEFT)
    {
        log_mutex.lock();
        logFile << log_time() << "[Obstacle Avoiding] Avoiding an obstacle - choosed left side to avoid" << endl;
        log_mutex.unlock();
        avoid_simple_obstacle_left_side();
    }
    log_mutex.lock();
    logFile << log_time() << "[Obstacle Avoiding] Obstacle Avoided Successfully!" << endl;
    log_mutex.unlock();
    auto end_time = std::chrono::steady_clock::now(); 
    time_skipped = (end_tts - start_tts_avoid) + (end_time - start_time) * 1.2; //'(' ')' for better visibility in code
    logFile << log_time() << "[Obstacle Avoiding] Time Skipped: " << time_skipped.count() << endl;
    return time_skipped;
}



void ObstacleAvoidance::simulate_real_case()
{
    robotVisionVoyager->move_forward();
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);

    while(true)
    {
        get_ultrasonic_data();
        if(DANGER_DISTANCE_THRESHOLD > ultrasonic_data)
        {
            Direction side = choose_avoiding_side();

            if (side == Direction::RIGHT)
            {
                avoid_simple_obstacle_right_side();
                logFile << log_time() << "right" << endl;
            }
            else if (side == Direction::LEFT)
            {
                avoid_simple_obstacle_left_side();
                logFile << log_time() << "left" << endl;
            }
            else 
            {
                /* special case -> maybe wait for the object to pass or be removed (?) */
                this_thread::sleep_for(std::chrono::milliseconds(5000));
            }

            robotVisionVoyager->stop();
            break;
        }
    }

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

            if (command_arg == DIR_MIN)
            {
                command_arg = DIR_MAX;
            }
            else if (command_arg == DIR_MAX)
            {
                command_arg = DIR_MIN;
            }
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
