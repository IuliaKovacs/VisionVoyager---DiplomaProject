
#include "ApplicationModule/application.h"


using namespace std;

ofstream logFile;
mutex mtx;
mutex log_mutex;
condition_variable cond_v;
atomic<bool> should_stop(false); 
atomic<bool> route_complete(false); 

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
    logFile << " - START MAIN APPLICATION  " << log_time() << endl << endl;
    KeyboardControl::initialize_keyboard_control();
    RouteRegistration::initialize_route_registration();
    CameraModule::initialize_camera_module();
    inititalize_language();
    initialize_route_display_files();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
}

void terminate_main_app()
{
    KeyboardControl::shutdown_keyboard_control();
    logFile << endl << endl << " - END MAIN APPLICATION  " << log_time() << endl;
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

    /* @ToDo - the building section should be calculated based on RFID reader info. So for now is hardcoded! */
    RouteRecordPlayer::set_current_section(Building_Section::SECTION_A);

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
    
    KeyboardControl::F11_listening_loop();
    bool Admin_Mode = KeyboardControl::get_F11_pressed();

    if(true == Admin_Mode)
    {   
        logFile << log_time() << "Starting Admin Mode Window" << endl;
        thread admin_mode_window_thread(TASK_ADMIN_MODE_WINDOW, argc, argv);
        admin_mode_window_thread.join();
    }

    /* ---- End of Admin Mode part ---- */



    /* ---- Start of Normal User Mode part ---- */

    // if(true != Admin_Mode)
    // {
    //     logFile << log_time() << "Starting Normal User Mode " << endl;

    //     /* StandBy/Sleep state until "start" or "hello" is recognized */
    //     VoiceRecognition::loop_recognition();

    //     display_hello_message();

    //     bool option_flag = false;
    //     string option;

    //     while(option_flag == false)
    //     {
    //         display_menu_options();
    //         option = VoiceRecognition::timed_listening_recognition_for_options();

    //         /* @ToDo - make sure that option can have only 3 values "ONE"/"TWO"/"UNKNOWN" */
    //         if(strcmp("UNKNOWN", option.c_str()) != 0)
    //         {
    //             if(strcmp("ONE", option.c_str()) == 0)
    //             {
    //                 display_option1_message();
    //                 option_flag = true;

    //                 /* Start executing in paralell the Line Following, Camera, Voice Recognition and RFID Reader Communication Tasks */
    //                 thread line_follower_thread(TASK_LINE_FOLLOWING);
    //                 thread camera_thread(TASK_CAMERA_MODULE);
    //                 thread reader_comm_thread(TASK_RFID_READER_COMM);
    //                 thread voice_recognition_thread(TASK_VOICE_RECOGNITION_WAIT);
    //                 line_follower_thread.join();
    //                 camera_thread.join();
    //                 reader_comm_thread.join();
    //                 voice_recognition_thread.join();
    //             }
    //             else 
    //             {   
    //                 display_option2_message();
    //                 option_flag = true;
                
    //                 /* Start executing in paralell the Route Playing, Camera + Obstacle Detectio, Voice Recognition and RFID Reader Communication Tasks */
                
    //             }
    //         }

    //     }
    // }

    
    /* ---- End of Normal User Mode part ---- */


    /* Thread testing part */

    thread route_player_thread(TASK_ROUTE_PLAYING, "../RouteDatabase/Section A/Secretariat.txt");
    // thread camera_thread(TASK_CAMERA_MODULE);
    // thread reader_comm_thread(TASK_RFID_READER_COMM);
    thread voice_recognition_thread(TASK_VOICE_RECOGNITION_WAIT);
    route_player_thread.join();
    // camera_thread.join();
    // reader_comm_thread.join();
    voice_recognition_thread.join();

    terminate_main_app();

    return 0;
}
