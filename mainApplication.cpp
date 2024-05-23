
#include "ApplicationModule/application.h"
#include <map>

using namespace std;

ofstream logFile;
mutex mtx;
mutex log_mutex;
condition_variable cond_v;
atomic<bool> should_stop(false); 
atomic<bool> route_complete(false);
atomic<bool> severe_error(false);
map<std::string, int> route_map_no = {
    {"ONE", 0},
    {"TWO", 1},
    {"THREE", 2}
};


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
    logFile.close();
}

int main(int argc, char *argv[]) 
{    
    initilize_main_app();

    /* pybind11 specific - starts the interpreter and maintains it alive */
    py::scoped_interpreter guard{}; 

    /* @ToDo make initialization look clean */
    VisionVoyager robot = VisionVoyager();
    RouteRecordPlayer::set_robot(&robot);
    KeyboardControl::set_robot(&robot);
    LineFollower::set_robot(&robot);
    ObstacleAvoidance::set_robot(&robot);

    /* --- Start Face Recognition Test --- */

    // CameraModule::create_csv_database_file();
    // std::string source_file = "../CameraModule/image.png";
    // std::string destination_file = "../CameraModule/image1.png";
    // std::ifstream source_stream(source_file, std::ios::binary);
    // std::ofstream destination_stream(destination_file, std::ios::binary);
    // destination_stream << source_stream.rdbuf();
    // source_stream.close();
    // destination_stream.close();
    // int result = CameraModule::recognize_face("../CameraModule/image1.png");
    // logFile << log_time() << "s" << result << endl;

    /* --- End Face Recognition Test --- */

    

    /* ---- Start of Admin Mode part ---- */
    
    // KeyboardControl::F11_listening_loop();
    // bool Admin_Mode = KeyboardControl::get_F11_pressed();
    // bool Admin_Mode = true;

    // if(true == Admin_Mode)
    // {   
    //     logFile << log_time() << "[MainApp] --- Starting Admin Mode Window ---" << endl;
    //     thread admin_mode_window_thread(ApplicationModule::TASK_ADMIN_MODE_WINDOW, argc, argv);
    //     thread safety_thread(ApplicationModule::TASK_SAFETY_MEASURES);
    //     admin_mode_window_thread.join();
    //     safety_thread.join();
    // }

    /* ---- End of Admin Mode part ---- */



    /* ---- Start of Normal User Mode part ---- */

    // if(true != Admin_Mode)
    // {
    //     logFile << log_time() << "[MainApp] --- Starting Normal User Mode ---" << endl;

    //     TextToSpeech::display_hello_message();

    //     /* StandBy/Sleep state until "start" or "hello" is recognized */
    //     VoiceRecognition::loop_recognition();
    //     logFile << log_time() << "[MainApp] Activation keyword recognized " << endl;
        

    //     bool option_flag = false;
    //     string option;

    //     while(option_flag == false)
    //     {
    //         TextToSpeech::display_menu_options();
    //         option = VoiceRecognition::timed_listening_recognition_for_options();

    //         if((strcmp("UNKNOWN", option.c_str()) != 0) && (strcmp("THREE", option.c_str()) != 0))
    //         {
    //             if(strcmp("ONE", option.c_str()) == 0)
    //             {   
    //                 string route_path = "";
    //                 logFile << log_time() << "[MainApp] Option 1 was selected (Route Player) " << endl;
    //                 option_flag = true;

    //                 while(route_path.empty())
    //                 {
    //                     TextToSpeech::display_option1_message();
    //                     /* @ToDo - make sure if there is no B section - A=ONE and C=THREE */
    //                     string section_option = VoiceRecognition::timed_listening_recognition_for_options();

    //                     if(strcmp("UNKNOWN", section_option.c_str()) != 0)
    //                     {
    //                         if((strcmp("ONE", section_option.c_str()) == 0) && (0 != RouteRegistration::get_section_A_routes().size())) 
    //                         {
    //                             TextToSpeech::display_section_A_options_message();
    //                             string section_route = VoiceRecognition::timed_listening_recognition_for_options();
    //                             int index = route_map_no[section_route];
    //                             route_path = RouteRegistration::get_section_A_routes()[index];
    //                             break;
    //                         }
    //                         else if((strcmp("TWO", section_option.c_str()) == 0) && (0 != RouteRegistration::get_section_B_routes().size()))
    //                         {
    //                             TextToSpeech::display_section_B_options_message();
    //                             string section_route = VoiceRecognition::timed_listening_recognition_for_options();
    //                             int index = route_map_no[section_route];
    //                             route_path = RouteRegistration::get_section_B_routes()[index];
    //                             break;
    //                         }
    //                         else if((strcmp("THREE", section_option.c_str()) == 0) && (0 != RouteRegistration::get_section_C_routes().size()))
    //                         {
    //                             TextToSpeech::display_section_C_options_message();
    //                             string section_route = VoiceRecognition::timed_listening_recognition_for_options();
    //                             int index = route_map_no[section_route];
    //                             route_path = RouteRegistration::get_section_C_routes()[index];
    //                             break;
    //                         }

    //                         TextToSpeech::display_repeat_message();
    //                     }
    //                 }

    //                 logFile << log_time() << "[MainApp] The selected route is: \"" << route_path << "\"" << endl;

    //                 /* Start executing in paralell the Route Playing, Camera + Obstacle Detectio, Voice Recognition and RFID Reader Communication Tasks */
    //                 thread route_player_thread(ApplicationModule::TASK_ROUTE_PLAYING, route_path);
    //                 // thread camera_thread(ApplicationModule::TASK_CAMERA_MODULE);
    //                 // thread reader_comm_thread(ApplicationModule::TASK_RFID_READER_COMM);
    //                 thread voice_recognition_thread(ApplicationModule::TASK_VOICE_RECOGNITION_WAIT);
    //                 route_player_thread.join();
    //                 // camera_thread.join();
    //                 // reader_comm_thread.join();
    //                 voice_recognition_thread.join();

    //                 break;
    //             }
    //             else if(strcmp("TWO", option.c_str()) == 0)
    //             {   
    //                 logFile << log_time() << "[MainApp] Option 2 was selected (Line Follower) " << endl;
    //                 TextToSpeech::display_option2_message();
    //                 option_flag = true;

    //                 /* Start executing in paralell the Line Following, Camera, Voice Recognition and RFID Reader Communication Tasks */
    //                 thread line_follower_thread(ApplicationModule::TASK_LINE_FOLLOWING);
    //                 thread camera_thread(ApplicationModule::TASK_CAMERA_MODULE);
    //                 thread reader_comm_thread(ApplicationModule::TASK_RFID_READER_COMM);
    //                 thread voice_recognition_thread(ApplicationModule::TASK_VOICE_RECOGNITION_WAIT);
    //                 line_follower_thread.join();
    //                 camera_thread.join();
    //                 reader_comm_thread.join();
    //                 voice_recognition_thread.join();

    //                 break;
    //             }
    //         }
    //         TextToSpeech::display_repeat_message();
    //     }
    // }

    
    /* ---- End of Normal User Mode part ---- */


    /* Thread testing part */

    thread route_player_thread(ApplicationModule::TASK_ROUTE_PLAYING, "../RouteDatabase/Section A/Secretariat.txt");
    thread safety_thread(ApplicationModule::TASK_SAFETY_MEASURES);
    // thread camera_thread(ApplicationModule::TASK_CAMERA_MODULE);
    // thread reader_comm_thread(ApplicationModule::TASK_RFID_READER_COMM, "../RouteDatabase/Section A/Secretariat");
    // thread voice_recognition_thread(ApplicationModule::TASK_VOICE_RECOGNITION_WAIT);
    route_player_thread.join();
    safety_thread.join();
    // camera_thread.join();
    // reader_comm_thread.join();
    // voice_recognition_thread.join();

    terminate_main_app();

    return 0;
}
