
#include "ApplicationModule/application.h"
#include <map>

using namespace std;

ofstream logFile;
mutex mtx;
mutex log_mutex;
mutex tts_mutex;
mutex speak_mutex;
mutex camera_mutex;
atomic<bool> should_stop(false); 
atomic<bool> route_complete(false);
atomic<bool> severe_error(false);
atomic<bool> moving(false);
atomic<bool> avoiding(false);
SevereErrorType error_type = SevereErrorType::NO_ERROR;
map<std::string, int> route_map_no = {
    {"ONE", 0},
    {"TWO", 1},
    {"THREE", 2}
};
condition_variable waiting_condition;
condition_variable speaking_condition;
condition_variable camera_condition;
atomic<bool> speak(false);
string global_message;
GuidingMode guiding_mode = GuidingMode::NO_GUIDING;





const char* vv_art = R"(                                                                                                                        

  _   __   _         _                   _   __                                    
 | | / /  (_)  ___  (_) ___   ___       | | / / ___   __ __ ___ _  ___ _ ___   ____
 | |/ /  / /  (_-< / / / _ \ / _ \      | |/ / / _ \ / // // _ `/ / _ `// -_) / __/
 |___/  /_/  /___//_/  \___//_//_/      |___/  \___/ \_, / \_,_/  \_, / \__/ /_/   
                                                    /___/        /___/             
                                          
)";


const char* signiture = R"(                                                                                                                        


     ________     _____        _  _          _  __                             
    /  ____  \   |_   _|      | |(_)        | |/ /                             
   /  / ___|  \    | |  _   _ | | _   __ _  | ' /  ___ __   __ __ _   ___  ___ 
  |  | |       |   | | | | | || || | / _` | |  <  / _ \\ \ / // _` | / __|/ __|
  |  | |___    |  _| |_| |_| || || || (_| | | . \| (_) |\ V /| (_| || (__ \__ \
   \  \____|  /  |_____|\__,_||_||_| \__,_| |_|\_\\___/  \_/  \__,_| \___||___/
    \________/                                                                 
                                                                                                                  
)";


string log_time()
{
    time_t now = time(nullptr);
    tm *tm_now = localtime(&now);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
    chrono::system_clock::now().time_since_epoch()).count() % 1000;


    ostringstream time_string;
    time_string << put_time(tm_now, "%d-%b-%Y") << "-";
    time_string << std::put_time(tm_now, "%Hh:%Mm:%Ss") << ":";
    time_string << std::setw(3) << std::setfill('0') << milliseconds << "ms  -  ";

    return time_string.str();
}

string generate_log_filename()
{
    time_t now = time(nullptr);
    tm *tm_now = localtime(&now);

    ostringstream filename;
    filename << "log_" << put_time(tm_now, "%Y_%b_%d") << "_time_";

    filename << put_time(tm_now, "%H_%M");

    return filename.str();
}

void initialize_log_file() 
{
    string log_filename = generate_log_filename();
    logFile.open(LOGS_PATH + log_filename + ".txt", ios::out | ios::trunc);

    if (!logFile.is_open()) 
    {
        logFile << log_time() << "Error: Opening the log file for writing failed!" << endl;
    }
}

void initilize_main_app()
{   
    initialize_log_file();
    logFile << vv_art;
    logFile << " - START OF MAIN APPLICATION  " << log_time() << endl << endl;
    KeyboardControl::initialize_keyboard_control();
    RouteRegistration::initialize_route_registration();
    CameraModule::initialize_camera_module();
    TextToSpeech::inititalize_language();
    TextToSpeech::initialize_route_display_files();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    logFile << log_time() << "[MainApp] Application initialization complete!" << endl; 
}

void terminate_main_app()
{
    KeyboardControl::shutdown_keyboard_control();
    logFile << endl << endl << " - END OF MAIN APPLICATION  " << log_time() << endl;
    logFile << endl << endl << signiture;
    logFile.close();
}

/*
string select_route_and_start()
{   
    string route_path = "";
    TextToSpeech::display_option1_message();
    string section_option = VoiceRecognition::loop_listening_for_choices();

    if(strcmp("UNKNOWN", section_option.c_str()) != 0)
    {
        if((strcmp("ONE", section_option.c_str()) == 0) && (0 != RouteRegistration::get_section_A_routes().size())) 
        { 
            while(true)
            {
                TextToSpeech::display_section_A_options_message();
                string section_route = VoiceRecognition::loop_listening_for_choices();
                int index = route_map_no[section_route];
                if(index < RouteRegistration::get_section_A_routes().size())
                {   
                    route_path = RouteRegistration::get_section_A_routes()[index];
                    return route_path;
                }
                TextToSpeech::display_repeat_message();
            }
        }
        else if((strcmp("TWO", section_option.c_str()) == 0) && (0 != RouteRegistration::get_section_B_routes().size()))
        {
        
            while(true)
            {
                TextToSpeech::display_section_B_options_message();
                string section_route = VoiceRecognition::loop_listening_for_choices();
                int index = route_map_no[section_route];
                if(index < RouteRegistration::get_section_B_routes().size())
                {
                    route_path = RouteRegistration::get_section_B_routes()[index];
                    return route_path;
                }
                TextToSpeech::display_repeat_message();
            }
        }
        else if((strcmp("THREE", section_option.c_str()) == 0) && (0 != RouteRegistration::get_section_C_routes().size()))
        {
            
            while(true)
            {
                TextToSpeech::display_section_C_options_message();
                string section_route = VoiceRecognition::loop_listening_for_choices();
                int index = route_map_no[section_route];
                if(index < RouteRegistration::get_section_C_routes().size())
                {   
                    route_path = RouteRegistration::get_section_C_routes()[index];
                    return route_path;
                }
                TextToSpeech::display_repeat_message();
            }
        }
    }

    return route_path;
}
*/



int main(int argc, char *argv[]) 
{    
    initilize_main_app();
    

#ifndef USE_SIMULATION    
    /* pybind11 specific - starts the interpreter and maintains it alive */
    py::scoped_interpreter guard{}; 
    VisionVoyager* robot = new VisionVoyager();
#else
    rclcpp::init(argc, argv);
    auto ros_node = std::make_shared<rclcpp::Node>("vision_voyager_node");
    VisionVoyager* robot = new VisionVoyager(ros_node);

    std::cout << "[ROS2] Node-ul ROS 2 a fost creat." << std::endl;
#endif

    robot->set_direction_limits(DIR_MIN_RG, DIR_MAX_RG);
    RouteRecordPlayer::set_robot(robot);
    KeyboardControl::set_robot(robot);
    LineFollower::set_robot(robot);
    ObstacleAvoidance::set_robot(robot);

    int counter = 0; 
    int stop_counter = -1;
    rclcpp::TimerBase::SharedPtr publish_timer;
    rclcpp::TimerBase::SharedPtr timer;

    publish_timer = ros_node->create_wall_timer(
        std::chrono::milliseconds(10),
        [robot]() 
        {
            robot->publish_all(); 
        });
    
    std::thread ros_thread([&](){
        rclcpp::spin(ros_node);
    });
    

    // thread route_player_thread(ApplicationModule::TASK_ROUTE_PLAYING, RouteRegistration::get_section_C_routes()[0]);
    // route_player_thread.join();

     /* ---- Start of Admin Mode part ---- */
    
    KeyboardControl::START_ADMIN_listening_loop();
    bool Admin_Mode = KeyboardControl::get_ADMIN_START_pressed();

    if(true == Admin_Mode)
    {   
        logFile << log_time() << "[MainApp] --- Starting Admin Mode Window ---" << endl;
        thread admin_mode_thread(ApplicationModule::TASK_ADMIN_MODE_WINDOW, argc, argv);
        admin_mode_thread.join();
    }

    rclcpp::shutdown();
    ros_thread.join();  
    terminate_main_app();

    return 0;
}


