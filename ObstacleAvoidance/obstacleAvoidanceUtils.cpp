

#include "obstacleAvoidanceUtils.h"
#include "../RouteRecordPlayer/routeRecordPlayUtils.h"


using namespace std;

VisionVoyager* ObstacleAvoidance::robotVisionVoyager = nullptr;
float ObstacleAvoidance::ultrasonic_data;


void ObstacleAvoidance::set_robot(VisionVoyager* robot)
{
    ObstacleAvoidance::robotVisionVoyager = robot;
}

void ObstacleAvoidance::get_ultrasonic_data()
{
    ObstacleAvoidance::ultrasonic_data = robotVisionVoyager->read_ultrasonic_data();
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
    logFile << ultrasonic_data << endl;
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
    logFile << ultrasonic_data << endl;
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

    /* ToDo special case of complex avoiding*/
}

void ObstacleAvoidance::avoid_simple_obstacle_right_side()
{
    robotVisionVoyager->move_forward();

    robotVisionVoyager->turn_right_max();
    this_thread::sleep_for(std::chrono::milliseconds(1393));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
    this_thread::sleep_for(std::chrono::milliseconds(460));
    robotVisionVoyager->turn_left_max();
    this_thread::sleep_for(std::chrono::milliseconds(1250));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
    this_thread::sleep_for(std::chrono::milliseconds(1500));
    robotVisionVoyager->turn_left_max();
    this_thread::sleep_for(std::chrono::milliseconds(1393));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
    this_thread::sleep_for(std::chrono::milliseconds(460));
    robotVisionVoyager->turn_right_max();
    this_thread::sleep_for(std::chrono::milliseconds(1250));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);

    this_thread::sleep_for(std::chrono::milliseconds(500)); 
    robotVisionVoyager->stop();
}



void ObstacleAvoidance::avoid_simple_obstacle_left_side()
{
    robotVisionVoyager->move_forward();
   
    robotVisionVoyager->turn_left_max();
    this_thread::sleep_for(std::chrono::milliseconds(1393));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
    this_thread::sleep_for(std::chrono::milliseconds(460));
    robotVisionVoyager->turn_right_max();
    this_thread::sleep_for(std::chrono::milliseconds(1250));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
    this_thread::sleep_for(std::chrono::milliseconds(1500));
    robotVisionVoyager->turn_right_max();
    this_thread::sleep_for(std::chrono::milliseconds(1393));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);
    this_thread::sleep_for(std::chrono::milliseconds(460));
    robotVisionVoyager->turn_left_max();
    this_thread::sleep_for(std::chrono::milliseconds(1250));
    robotVisionVoyager->set_dir_angle(DEFAULT_WHEEL_ANGLE);

    this_thread::sleep_for(std::chrono::milliseconds(500)); 
    robotVisionVoyager->stop();
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
                logFile << "right" << endl;
            }
            else if (side == Direction::LEFT)
            {
                avoid_simple_obstacle_left_side();
                logFile << "left" << endl;
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

        RouteRecordPlayer::play_command(command_name, command_arg);
        // logFile << command_name << "(" << command_arg << ")" << endl;
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

        RouteRecordPlayer::play_command(command_name, command_arg);
        this_thread::sleep_for(std::chrono::milliseconds(milliseconds)); 
    }

    robotVisionVoyager->stop();
}
