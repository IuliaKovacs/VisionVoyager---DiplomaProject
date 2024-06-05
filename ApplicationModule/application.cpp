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

using namespace std;
using namespace OpenXLSX;

bool ApplicationModule::TASK_LINE_FOLLOWING()
{
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
        camera_condition.wait(lock, []{ return (!moving.load()); });
        string source_file =  CameraModule::capture_image(CAPTURED_IMAGES_PATH);
        log_mutex.lock();
        logFile << log_time() << LOG_THREAD_CAMERA_PREFIX << " Took a photo " << endl;
        log_mutex.unlock();
        string destination_file = TEMP_CAPTURED_IMG;
        ifstream source_stream(source_file, std::ios::binary);
        ofstream destination_stream(destination_file, std::ios::binary);
        destination_stream << source_stream.rdbuf();
        source_stream.close();
        destination_stream.close();
        // process image and display message if it is a recognized person in it 
        int result = CameraModule::recognize_face(TEMP_CAPTURED_IMG);
        log_mutex.lock();
        logFile << log_time() << LOG_THREAD_CAMERA_PREFIX << " Label result: s" << result << endl;
        log_mutex.unlock();
        if(-1 != result)
        {
            //@ToDo RO + EN
            Person recognized_person = CameraModule::get_person_by_id("s" + to_string(result));
            string tts_person = "Persoana: \n" + recognized_person.get_first_name() + "  " + recognized_person.get_last_name();
            log_mutex.lock();
            logFile << log_time() << LOG_THREAD_CAMERA_PREFIX << " Recogized person: " << recognized_person.get_first_name() + "  " + recognized_person.get_last_name() << endl;
            log_mutex.unlock();
            tts_person = tts_person + " este lângă tine. \n\n\n Dacă ai întrebări este la dispoziția ta!" + "\n\n\n  Rol: " + recognized_person.get_role();
            TextToSpeech::display_custom_message(tts_person); 
        }
    }

    // @ToDo - after the route is completed - check for 10 seconds if a person from staff is recognized
}

bool ApplicationModule::TASK_RFID_READER_COMM(optional<string> route_name)
{
    uint8_t out_data;
    RFID_request_status_t st;
    RFID_Tag_Information tag_info;
    bool system_init_status;

    logFile << log_time() <<  LOG_RFID_PREFIX << " Start of RFID Reader Communication Thread " << endl;

    RFID_init();

    st = RFID_Send_Ping();
    logFile << log_time() <<  LOG_RFID_PREFIX << " Ping Request Status = " << st << endl;

    
    if(st == RFID_REQUEST_OK)
    {   
        char last_room_name[20] = ""; //@ToDo

        st = RFID_get_System_Init_Status(&system_init_status);
        if((st == RFID_REQUEST_OK) && (system_init_status == true))
        {   
            st = RFID_get_Rooms(&tag_info);
            logFile << log_time() <<  LOG_RFID_PREFIX << " Room Request Status = " << st << endl;

            if(st == RFID_REQUEST_OK)
            {   
                if(strcmp(last_room_name, tag_info.room_name) != 0)
                {   
                    string room_description_message;
                    logFile << log_time() << LOG_RFID_PREFIX << " Room name: " << tag_info.room_name << endl;
                    logFile << log_time() << LOG_RFID_PREFIX << " Room description: " << tag_info.room_description << endl;
                    logFile << log_time() << LOG_RFID_PREFIX << " Room destination_status: " << tag_info.destination_node << endl;
                    if(Language::EN == TextToSpeech::get_language())
                    {
                        if(false == tag_info.destination_node)
                        {
                            room_description_message += "We pass by " + string(tag_info.room_name) + "\n\n\n\n\n\n";
                            room_description_message += "Use the waiting command to stop here! \n\n\n\n\n\n";
                        }
                        else
                        {   
                            room_description_message += "We reached " + string(tag_info.room_name) + "\n\n\n\n\n\n";
                            room_description_message += "This is your destination! \n\n\n\n\n\n";
                        }
                        room_description_message += string(tag_info.room_description) + "\n\n\n\n\n\n";
                    }
                    else
                    {
                        if(false == tag_info.destination_node)
                        {
                            room_description_message += "Trecem pe lângă " + string(tag_info.room_name) + "\n\n\n\n\n\n";
                            room_description_message += "Folosește comanda de așteptare dacă dorești să ne oprim aici! \n\n\n\n\n\n";
                        }
                        else
                        {   
                            room_description_message += "Am ajuns la: " + string(tag_info.room_name) + "\n\n\n\n\n\n";
                            room_description_message += "Aceasta este destinația ta! \n\n\n\n\n\n";
                        }
                        room_description_message += string(tag_info.room_description) + "\n\n\n\n\n\n";
                    }
                    strcpy(last_room_name, tag_info.room_name);
                    
                    /* Send data for displaying the tag info */
                    std::lock_guard<std::mutex> lock(speak_mutex);
                    global_message = room_description_message;
                    speak.store(true);
                    speaking_condition.notify_all();
                }

                string tag_info_route_name = string(tag_info.room_name);
                if(route_name.has_value())
                {
                    if((tag_info_route_name == RouteRegistration::get_route_name_from_path(route_name.value())) && (true == tag_info.destination_node))
                    {
                        route_complete.store(true);
                    }
                }
            }
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
    }
    else
    {
        return false;
    }

    // logFile << log_time() << LOG_RFID_PREFIX << " Ended successfully! " << endl;
    return true;
}


bool ApplicationModule::TASK_ROUTE_PLAYING(string route_name)
{
     RouteRecordPlayer::play_route_conditioned(route_name);
     log_mutex.lock();
     logFile << log_time() << LOG_THREAD_ROUTE_PLAYER_PREFIX << " --- Route Aborted Successfully --- " << endl;
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
            VoiceRecognition::loop_recognition();
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