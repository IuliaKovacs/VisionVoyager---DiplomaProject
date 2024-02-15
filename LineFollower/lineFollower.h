#ifndef LINE_FOLLOWER
#define LINE_FOLLOWER

#include "../VisionVoyagerMoves/visionVoyager.h"
#include <vector>


#define LINE_THRESHOLD 80
#define STOP_VALUE 350
#define IN_AIR_THRESHOLD 15


using namespace std;


enum class State 
{
    UNKNOWN,
    FORWARD,
    LEFT,
    RIGHT
};


class LineFollower
{   
    static VisionVoyager* robotVisionVoyager;
    static vector<int> grayscale_data;
    static int stop_counter;
    static State last_state;
    static void get_grayscale_data();
    static vector<int> get_line_from_grayscale_data();
    static bool verify_stop_condition();
    static bool verify_is_in_air();
    static void search_line();
    static State compute_next_state(vector<int> grayscale_line_status);
    static void execute_move(State current_state);


public:
    static void setRobot(VisionVoyager* robot);
    static void follow_line();
};


#endif //LINE_FOLLOWER
