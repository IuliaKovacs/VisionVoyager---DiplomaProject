#include "lineFollower.h"

using namespace std;

VisionVoyager* LineFollower::robotVisionVoyager = nullptr;
vector<int> LineFollower::grayscale_data;

void LineFollower::setRobot(VisionVoyager* robot)
{
    LineFollower::robotVisionVoyager = robot;
}

void LineFollower::get_grayscale_data()
{
    LineFollower::grayscale_data = robotVisionVoyager->read_grayscale_data();
}


vector<int> LineFollower::get_line_from_grayscale_data()
{
    vector<int> line_status = {
        static_cast<int>(grayscale_data[0] < 80),
        static_cast<int>(grayscale_data[1] < 80),
        static_cast<int>(grayscale_data[2] < 80)
    };
    
    return line_status;
}


void LineFollower::follow_line()
{
    while(true)
    {
        get_grayscale_data();
        cout << grayscale_data[0] << "  " << grayscale_data[1] << "  " << grayscale_data[2] << endl;
        // get_line_from_grayscale_data();
    }
}