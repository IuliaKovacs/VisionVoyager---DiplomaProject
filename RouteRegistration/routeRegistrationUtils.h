#include <string>
#include <vector>
#include <optional>


#define REGISTRATION_DISABLED false
#define REGISTRATION_ENABLED true
 
using namespace std;


enum class MovingState {
    STATIONARY,
    MOVING_FORWARD,
    REVERSING
};


class RouteRegistration
{
    bool register_enabled;
    vector<string> route_names;  
    /* vector<string> route_file_names; */  
    /* @ToDo convention that the file name uses route_name with suffix .txt (?) */
    string current_route_name;
    MovingState moving_state;
    
public:

    RouteRegistration();
    string get_current_timestamp();
    void set_moving_state(MovingState moving_state);
    bool create_route_file(string route_name);
    int extract_the_miliseconds_between_two_commands(string command_1, string command_2);
    void register_current_move(string move_name, optional<int> arg_value = nullopt);
    void register_speed_change(int new_speed);
    void delete_duplicate_line_commands(); 
    void create_record_for_route(); 
    void end_registration();
    void set_register_enabled_true();
};