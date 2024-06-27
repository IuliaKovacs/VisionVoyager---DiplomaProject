#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include "../VisionVoyagerGUI/adminModeWindow.h"
#include "application.h"
#include "../RFID/RFID_Manager.h"
#include <chrono>
#include <ctime>
#include <OpenXLSX.hpp>

#define CAPTURED_IMAGES_PATH "../CameraModule/CapturedImages"
#define TEMP_CAPTURED_IMG "../CameraModule/CapturedImages/temp.png"
#define MILISECONDS_FOR_CAMERA_RECOGNITION 1000 // ms - miliseconds

using namespace std;
using namespace OpenXLSX;

bool ApplicationModule::TASK_LINE_FOLLOWING()
{   

    tts_mutex.lock();
    if(Language::EN == TextToSpeech::get_language())
    {   
        TextToSpeech::display_custom_message("Say ONE if you want to record the route");
    }
    else
    {
        TextToSpeech::display_custom_message("Rostește ONE dacă vrei să înregistrăm traseul");
    }
    tts_mutex.unlock();
    string option = VoiceRecognition::loop_listening_for_choices();
    if("ONE" == option)
    {
        RouteRegistration::set_register_enabled_true();
    }
    tts_mutex.lock();
    if(Language::EN == TextToSpeech::get_language())
    {   
        TextToSpeech::display_custom_message("Use the activation command in order to start!");
    }
    else
    {
        TextToSpeech::display_custom_message("Rostește un cuvânt de activare ca să pornim!");
    }
    tts_mutex.unlock();
    VoiceRecognition::loop_recognition_for_start();
    RouteRegistration::set_register_enabled_true();
    LineFollower::follow_line();
    return true;
}

void ApplicationModule::TASK_CAMERA_MODULE()
{
    log_mutex.lock();
    logFile << log_time() << LOG_THREAD_CAMERA_PREFIX << " Camera Thread Started " << endl;
    log_mutex.unlock();
    CameraModule::create_csv_database_file();

    while(!route_complete.load())
    {
        std::unique_lock<std::mutex> lock(camera_mutex);
        camera_condition.wait(lock, []{ return (!moving.load() || route_complete.load()); });
        capture_photo_and_send_to_process();
    }

    /* after the route is completed - check for 10 seconds if a person from staff is recognized */
    auto end_timer = std::chrono::steady_clock::now() + std::chrono::milliseconds(MILISECONDS_FOR_CAMERA_RECOGNITION);

    while ((std::chrono::steady_clock::now() < end_timer)) 
    {
        capture_photo_and_send_to_process();
    }

    log_mutex.lock();
    logFile << log_time() << LOG_THREAD_CAMERA_PREFIX << "--- Task Ended ---" << endl;
    log_mutex.unlock();
}

bool ApplicationModule::TASK_RFID_READER_COMM(optional<string> route_name)
{
    return true;
}


bool ApplicationModule::TASK_ROUTE_PLAYING(string route_name)
{
     RouteRecordPlayer::play_route_conditioned(route_name);
     log_mutex.lock();
     logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " --- Task Ended --- " << endl;
     log_mutex.unlock();
     return true;
}

bool ApplicationModule::TASK_ADMIN_MODE_WINDOW(int argc, char *argv[]) 
{
    start_GUI(argc, argv);
    return true;
}

bool ApplicationModule::TASK_VOICE_RECOGNITION_WAIT()
{
    log_mutex.lock();
    logFile << log_time() << LOG_THREAD_VOICE_PREFIX << " Voice Regognition Thread Started " << endl;
    log_mutex.unlock();

    while(!route_complete.load())
    {   
        /* Listening in case the user input is "wait" */
        VoiceRecognition::loop_listening_for_wait();
        /* Check if route is already completely pursued */
        if (!route_complete.load())
        {
            /* Listening for the keyword "start" in order to continue the guidigng */
            VoiceRecognition::loop_recognition_for_start();
        }
    }

    return true;
}

bool ApplicationModule::TASK_SAFETY_MEASURES()
{
    SevereErrorType err_type = RouteRecordPlayer::check_motors_feedback();

    if(SevereErrorType::MOTOR_ERROR == err_type)
    {
        tts_mutex.lock();
        if(Language::EN == TextToSpeech::get_language())
        {   
            TextToSpeech::display_custom_message("Severe motor issue! \n\n\n Aborting the guiding process! \n\n\n Please contact the building staff!");
        }
        else
        {
            TextToSpeech::display_custom_message("Problema severă la motoare \n\n\n Abandonare proces de ghidare \n\n\n Contactați personalul clădirii!");
        }
        tts_mutex.unlock();
    }
    if(SevereErrorType::LOW_VOLTAGE == err_type)
    {   
        tts_mutex.lock();
        if(Language::EN == TextToSpeech::get_language())
        {   
            TextToSpeech::display_custom_message("LOW VOLTAGE! \n\n\n LOW VOLTAGE! \n\n\n Please contact the building staff!");
        }
        else
        {
            TextToSpeech::display_custom_message("Baterie descărcată! \n\n\n Baterie descărcată! \n\n\n Contactați personalul clădirii!");
        }
        tts_mutex.unlock();
    }

    
    return true;
}


bool ApplicationModule::TASK_SPEAKING()
{
    log_mutex.lock();
    logFile << log_time() << LOG_THREAD_SPEAK_PREFIX << " TTS Thread Started " << endl;
    log_mutex.unlock();

    while(!route_complete.load())
    {
        std::unique_lock<std::mutex> lock(speak_mutex);
        speaking_condition.wait(lock, []{ return (speak.load() || route_complete.load()); });
        if(!global_message.empty())
        {
            log_mutex.lock();
            logFile << log_time() << LOG_THREAD_SPEAK_PREFIX << "Custom message is displayed" << endl;
            log_mutex.unlock(); 
            TextToSpeech::display_custom_message(global_message);
            speak.store(false);
            global_message = "";
        }
    }
}


void ApplicationModule::increment_excel_route_count(string route_path)
{
    string route = RouteRegistration::get_route_name_from_path(route_path);
    size_t found = route.find(".txt");
    if (found != std::string::npos) {
        route.erase(found, 4);
    }

    XLDocument doc;
    doc.open(ROUTE_DATA_EXCEL_PATH);
    auto wks = doc.workbook().worksheet(SHEET_ROUTE_NAME);

    QBarSeries *series = new QBarSeries;

    for(int i = 2; i <= wks.rowCount(); i++)
    {   
        string route = wks.cell(i, 1).value().get<std::string>();

        if (route == route) 
        {
            int selection_counter = wks.cell(i, 3).value().get<int>();
            selection_counter++;
            wks.cell(i, 3).value() = selection_counter;
        }
    }

    doc.save();
    doc.close();
    logFile << log_time() << "[Data Collection] Counter update for \"" << route << "\" has been made - Excel Updated " << endl;
}



void ApplicationModule::capture_photo_and_send_to_process()
{
    string source_file =  CameraModule::capture_image(CAPTURED_IMAGES_PATH);
    logFile << log_time() << LOG_THREAD_CAMERA_PREFIX << " Took a photo " << endl;
    string destination_file = TEMP_CAPTURED_IMG;
    ifstream source_stream(source_file, std::ios::binary);
    ofstream destination_stream(destination_file, std::ios::binary);
    destination_stream << source_stream.rdbuf();
    source_stream.close();
    destination_stream.close();
    // process image and display message if it is a recognized person in it 
    int result = CameraModule::recognize_face(TEMP_CAPTURED_IMG);
    // logFile << log_time() << LOG_THREAD_CAMERA_PREFIX << " Label result: s" << result << endl;

    if(-1 != result)
    {
        Person recognized_person = CameraModule::get_person_by_id("s" + to_string(result));
        string tts_person = "Persoana: \n" + recognized_person.get_first_name() + "  " + recognized_person.get_last_name();
        log_mutex.lock();
        logFile << log_time() << LOG_THREAD_CAMERA_PREFIX << " Recogized person: " << recognized_person.get_first_name() + "  " + recognized_person.get_last_name() << endl;
        log_mutex.unlock();
        if(Language::EN == TextToSpeech::get_language())
        {   
            tts_person = tts_person + " is next to you if you need support!" + "\n\n\n  Role: " + recognized_person.get_role();
        }
        else
        {
            tts_person = tts_person + " este lângă tine. \n\n\n Dacă ai întrebări este la dispoziția ta!" + "\n\n\n  Rol: " + recognized_person.get_role();
        }
        TextToSpeech::display_custom_message(tts_person); 
    }
}



void ApplicationModule::MODE_1_ROUTE_PLAYING(string route_path)
{
    guiding_mode = GuidingMode::ROUTE_PLAYER_MODE;
    increment_excel_route_count(route_path); 
    string route = RouteRegistration::get_route_name_from_path(route_path);
    string msg = "";
    if(Language::EN == TextToSpeech::get_language())
    {   
        msg = "Attention! Start! \n\n\n I will start to guide you " + route;
    }
    else
    {
        msg = "Atenție, pornim!! \n\n\n Voi începe să redau ruta " + route;
    }
    TextToSpeech::display_custom_message(msg);

    route_path += ".txt";
    thread route_player_thread(ApplicationModule::TASK_ROUTE_PLAYING, route_path);
    thread voice_recognition_thread(ApplicationModule::TASK_VOICE_RECOGNITION_WAIT);
    thread safety_thread(ApplicationModule::TASK_SAFETY_MEASURES);
    thread speaking_thread(ApplicationModule::TASK_SPEAKING);
    route_player_thread.join();
    safety_thread.join();
    speaking_thread.join();
    voice_recognition_thread.join();
}

void ApplicationModule::MODE_2_LINE_FOLLOWER()
{
    guiding_mode = GuidingMode::LINE_FOLLOWER_MODE;
    string msg = "";
    if(Language::EN == TextToSpeech::get_language())
    {   
        msg = "Atenție, pornim!! \n\n\n Voi începe să urmăresc linia";
    }
    else
    {
        msg = "Atenție, pornim!! \n\n\n Voi începe să urmăresc linia";
    }
    thread safety_thread(ApplicationModule::TASK_SAFETY_MEASURES);
    thread line_follower_thread(ApplicationModule::TASK_LINE_FOLLOWING);
    thread speaking_thread(ApplicationModule::TASK_SPEAKING);
    thread voice_recognition_thread(ApplicationModule::TASK_VOICE_RECOGNITION_WAIT);
    line_follower_thread.join();
    safety_thread.join();
    speaking_thread.join();
    voice_recognition_thread.join();
}