#ifndef ROUTE_REGISTRATION_UTILS
#define ROUTE_REGISTRATION_UTILS

#include "../ApplicationModule/application_utils.h"
#include <string>
#include <vector>
#include <optional>


#define REGISTRATION_DISABLED false
#define REGISTRATION_ENABLED true
#define TIMESTAMP_HISTORY "Timestamp_Routes/"
 
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
    static vector<string> route_paths;  
    static string current_route_name;
    static MovingState moving_state;
    static vector<string> section_A_routes;
    static vector<string> section_B_routes;
    static vector<string> section_C_routes;
    static string get_current_timestamp();
    static bool create_route_file();
    static int extract_the_miliseconds_between_two_commands(string command_1, string command_2);
    static void delete_duplicate_line_commands(); 
    static void create_record_for_route(); 
    static string compute_new_route_name();
    static vector<string> split_path(const string& path); /* @ToDo check if still needed or has to be removed */
    static string get_section_from_path(const string& path); /* @ToDo check if still needed or has to be removed */

public:
    RouteRegistration();
    static void set_moving_state(MovingState moving_state);
    static void set_register_enabled_true();
    static vector<string>& get_route_paths();
    static string get_route_database_directory_path();
    static void initialize_route_registration();
    static void display_possible_routes();
    static void register_current_move(string move_name, optional<int> arg_value = nullopt);
    static void register_speed_change(int new_speed);
    static void end_registration(); 
    static void prepare_route_paths();
    static string get_route_name_from_path(const string& path);
    static vector<string> get_section_A_routes();
    static vector<string> get_section_B_routes();
    static vector<string> get_section_C_routes();
};

#endif  // ROUTE_REGISTRATION_UTILS