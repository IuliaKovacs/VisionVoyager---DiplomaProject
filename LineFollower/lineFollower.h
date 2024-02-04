#ifndef LINE_FOLLOWER
#define LINE_FOLLOWER

#include "../VisionVoyagerMoves/visionVoyager.h"

class LineFollower
{   
    static VisionVoyager* robotVisionVoyager;
    static void setRobot(VisionVoyager* robot);
    static void get_greyscale_data();
    static void get_line_from_grayscale_data();
    static void search_line();
    static void compute_next_state();
    static void execute_move();
    static bool verify_stop_condition(int stop_value);
    static bool verify_is_on_air();

public:
    static void follow_line();
};


#endif //LINE_FOLLOWER