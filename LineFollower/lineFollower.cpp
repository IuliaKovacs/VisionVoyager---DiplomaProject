#include "lineFollower.h"

using namespace std;

VisionVoyager* LineFollower::robotVisionVoyager = nullptr;

void LineFollower::setRobot(VisionVoyager* robot)
{
    KeyboardControl::robotVisionVoyager = robot;
}

