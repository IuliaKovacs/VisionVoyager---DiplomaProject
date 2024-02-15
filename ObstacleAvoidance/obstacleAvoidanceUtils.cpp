
#include "obstacleAvoidanceUtils.h"

using namespace std;

VisionVoyager* ObstacleAvoidance::robotVisionVoyager = nullptr;
float ObstacleAvoidance::ultrasonic_data;


void ObstacleAvoidance::setRobot(VisionVoyager* robot)
{
    ObstacleAvoidance::robotVisionVoyager = robot;
}

void ObstacleAvoidance::get_ultrasonic_data()
{
    ObstacleAvoidance::ultrasonic_data = robotVisionVoyager->read_ultrasonic_data();
    cout<<ultrasonic_data<<endl;
}


void ObstacleAvoidance::avoid_simple_obstacle_right_side()
{
    robotVisionVoyager->move_forward();

    while(true)
    {   
        get_ultrasonic_data();
        cout << ultrasonic_data << "   ";
        if(DANGER_DISTANCE_THRESHOLD > ultrasonic_data)
        {
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

            break;
        }
    }

    this_thread::sleep_for(std::chrono::milliseconds(1000)); 
    robotVisionVoyager->stop();
}



void ObstacleAvoidance::avoid_simple_obstacle_left_side()
{
    robotVisionVoyager->move_forward();

    while(true)
    {   
        get_ultrasonic_data();
        cout << ultrasonic_data << "   ";
        if(DANGER_DISTANCE_THRESHOLD > ultrasonic_data)
        {
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

            break;
        }
    }

    this_thread::sleep_for(std::chrono::milliseconds(1000)); 
    robotVisionVoyager->stop();
}




void ObstacleAvoidance::reverse_route(string route_name)
{

}