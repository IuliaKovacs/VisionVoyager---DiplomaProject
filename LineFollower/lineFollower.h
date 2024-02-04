#ifndef LINE_FOLLOWER
#define LINE_FOLLOWER

#include "../VisionVoyagerMoves/visionVoyager.h"
#include <vector>


using namespace std;


class LineFollower
{   
    static VisionVoyager* robotVisionVoyager;
    static vector<int> grayscale_data;
    static void get_grayscale_data();
    static vector<int> get_line_from_grayscale_data();
    // static void search_line();
    // static void compute_next_state();
    // static void execute_move();
    // static bool verify_stop_condition(int stop_value);
    // static bool verify_is_on_air();

public:
    static void setRobot(VisionVoyager* robot);
    static void follow_line();
};


#endif //LINE_FOLLOWER