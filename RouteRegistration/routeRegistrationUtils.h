#ifndef ROUTE_REGISTRATION_UTILS
#define ROUTE_REGISTRATION_UTILS

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
    static bool register_enabled;
    static string route_database_directory_path;
    static vector<string> route_names;  
    static string current_route_name;
    static MovingState moving_state;
    static string get_current_timestamp();
    static bool create_route_file(string route_name);
    static int extract_the_miliseconds_between_two_commands(string command_1, string command_2);
    static void delete_duplicate_line_commands(); 
    static void create_record_for_route(); 

public:
    RouteRegistration();
    static void set_moving_state(MovingState moving_state);
    static void set_register_enabled_true();
    static void register_current_move(string move_name, optional<int> arg_value = nullopt);
    static void register_speed_change(int new_speed);
    static void end_registration();
 
};

#endif  // ROUTE_REGISTRATION_UTILS