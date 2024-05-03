#include <string>
#include <vector>
#include "textToSpeach.h"
#include "../RouteRegistration/routeRegistrationUtils.h"

#define VISION_VOYAGER_RO_VOICE "vv"
#define VISION_VOYAGER_EN_VOICE "mb-en1"
#define LANGUAGE_FILE_PATH "../TextToSpeach/language.txt"

#define MODE_OPTIONS_FILE(arg) \
    ( \
        (arg == Language::RO) ?  "\"../TextToSpeach/RO/mode_options.txt\"" : \
                                      "\"../TextToSpeach/EN/mode_options.txt\""  \
    )



#define OPTION1_MESSAGE_FILE(arg) \
    ( \
        (arg == Language::RO) ?  "\"../TextToSpeach/RO/option1.txt\"" : \
                                      "\"../TextToSpeach/EN/option1.txt\""  \
    )


#define OPTION2_MESSAGE_FILE(arg) \
    ( \
        (arg == Language::RO) ?  "\"../TextToSpeach/RO/option2.txt\"" : \
                                      "\"../TextToSpeach/EN/option2.txt\""  \
    )


#define HELLO_MESSAGE_FILE(arg) \
    ( \
        (arg == Language::RO) ?  "\"../TextToSpeach/RO/hello_message.txt\"" : \
                                      "\"../TextToSpeach/EN/hello_message.txt\""  \
    )

#define SECTION_A_MESSAGE_FILE(arg) \
    ( \
        (arg == Language::RO) ?  "\"../TextToSpeach/RO/section_A_options.txt\"" : \
                                      "\"../TextToSpeach/EN/section_A_options.txt\""  \
    )

#define SECTION_B_MESSAGE_FILE(arg) \
    ( \
        (arg == Language::RO) ?  "\"../TextToSpeach/RO/section_B_options.txt\"" : \
                                      "\"../TextToSpeach/EN/section_B_options.txt\""  \
    )

#define SECTION_C_MESSAGE_FILE(arg) \
    ( \
        (arg == Language::RO) ?  "\"../TextToSpeach/RO/section_C_options.txt\"" : \
                                      "\"../TextToSpeach/EN/section_C_options.txt\""  \
    )

#define REPEAT_MESSAGE_FILE(arg) \
    ( \
        (arg == Language::RO) ?  "\"../TextToSpeach/RO/repeat_selection.txt\"" : \
                                      "\"../TextToSpeach/EN/repeat_selection.txt\""  \
    )
    

#define CHOOSE_LANGUAGE(lang) ((lang == Language::RO) ? VISION_VOYAGER_RO_VOICE : VISION_VOYAGER_EN_VOICE)



using namespace std;
Language language = Language::UNKNOWN;
vector<string> voice_commands = {"\"ONE\"", "\"TWO\"", "\"THREE\""};

void set_language_RO(){
    
    if(Language::UNKNOWN != language)
    {   
        ofstream file(LANGUAGE_FILE_PATH, ofstream::trunc);
        if (file.is_open()) 
        {
            file << "RO";
            file.close();
            logFile << log_time() << "Language changed to RO" << endl;
        }
        else
        {
            logFile << log_time() << "Error: There is a problem with setting the language!" << endl;
        }
    }
    language = Language::RO;
}

void set_language_EN(){
    
    if(Language::UNKNOWN != language)
    {
        ofstream file(LANGUAGE_FILE_PATH, ofstream::trunc);
        if (file.is_open()) 
        {
            file << "EN";
            file.close();
            logFile << log_time() << "Language changed to EN" << endl;
        }
        else
        {
            logFile << log_time() << "Error: There is a problem with setting the language!" << endl;
        }
    }
    language = Language::EN;
}

void inititalize_language()
{
    ifstream file(LANGUAGE_FILE_PATH);
    if(file.is_open()) {
        string line;
        if(std::getline(file, line)) 
        {
            if("RO" == line)
            {   
                file.close();
                set_language_RO();
                logFile << log_time() << "[TTS] TTS inititalized with RO language" << endl;
            }
            else if("EN" == line)
            {   
                file.close();
                set_language_EN();
                logFile << log_time() << "[TTS] TTS inititalized with EN language" << endl;
            }
        } 
        else 
        {
            logFile << log_time() << "Error: There is a problem with initialization of the language!" << endl;
            file.close();
        }
    } 
    else 
    {
        logFile << log_time() << "Error: There is a problem with initialization of the language!" << endl;
    }
}

void display_menu_options()
{   
    logFile << log_time() << "[TTS] Displaying menu options " << endl;
    string command = "espeak -v " + string(CHOOSE_LANGUAGE(language)) + " -s 150 " + " -f " + string(MODE_OPTIONS_FILE(language));
    system(command.c_str());
}

void display_option1_message()
{
    string command = "espeak -v " + string(CHOOSE_LANGUAGE(language)) + " -s 150 " + " -f " + string(OPTION1_MESSAGE_FILE(language));
    system(command.c_str());
}

void display_option2_message()
{
    string command = "espeak -v " + string(CHOOSE_LANGUAGE(language)) + " -s 150 " + " -f " + string(OPTION2_MESSAGE_FILE(language));
    system(command.c_str());
}

void display_section_A_options_message()
{   
    logFile << log_time() << "[TTS] Displaying section A routes " << endl;
    string command = "espeak -v " + string(CHOOSE_LANGUAGE(language)) + " -s 150 " + " -f " + string(SECTION_A_MESSAGE_FILE(language));
    system(command.c_str());
}

void display_section_B_options_message()
{   
    logFile << log_time() << "[TTS] Displaying section B routes " << endl;
    string command = "espeak -v " + string(CHOOSE_LANGUAGE(language)) + " -s 150 " + " -f " + string(SECTION_B_MESSAGE_FILE(language));
    system(command.c_str());
}

void display_section_C_options_message()
{   
    logFile << log_time() << "[TTS] Displaying section C routes " << endl;
    string command = "espeak -v " + string(CHOOSE_LANGUAGE(language)) + " -s 150 " + " -f " + string(SECTION_C_MESSAGE_FILE(language));
    system(command.c_str());
}

void display_hello_message()
{   
    logFile << log_time() << "[TTS] Displaying hello message " << endl;
    string command = "espeak -v " + string(CHOOSE_LANGUAGE(language)) + " -s 150 " + " -f " + string(HELLO_MESSAGE_FILE(language));
    system(command.c_str());
}

void display_repeat_message()
{
    logFile << log_time() << "[TTS] Displaying repeat message " << endl;
    string command = "espeak -v " + string(CHOOSE_LANGUAGE(language)) + " -s 150 " + " -f " + string(REPEAT_MESSAGE_FILE(language));
    system(command.c_str());
}

void initialize_route_display_files()
{   
    initialize_section_options_file_RO();
    initialize_section_options_file_EN();
    initialize_route_display_files_RO();
    initialize_route_display_files_EN();
}


void initialize_section_options_file_RO()
{
    ofstream SectionOptionsFileRO("../TextToSpeach/RO/option1.txt", ios_base::trunc);
    if (SectionOptionsFileRO.is_open())
    {
        SectionOptionsFileRO << "Ai selectat opțiunea pentru urmărirea unei rute. Trebuie să selectezi o secțiune dintre următoarele:" << endl << endl;
        
        for(char c = 'A'; c <= 'C'; c++)
        {   
            vector<string> routes;
            switch (c) 
            {
                case 'A':
                    routes = RouteRegistration::get_section_A_routes();
                    break;
                case 'B':
                    routes = RouteRegistration::get_section_B_routes();
                    break;
                case 'C':
                    routes = RouteRegistration::get_section_C_routes();
                    break;
                default:
                    /* This should be unreachable */
                    break;
            }
        
            if(0 < routes.size())
            {
                SectionOptionsFileRO << "Secțiunea " <<  string(1, c) << endl << endl << endl;
                SectionOptionsFileRO << "   Această secțiune cuprinde următoarele săli: " << endl;

                for(int i = 1; i <= routes.size(); i++)
                {
                    SectionOptionsFileRO << "       " << RouteRegistration::get_route_name_from_path(routes[i-1]) << "." << endl << endl << endl;
                }

                SectionOptionsFileRO << "   Pentru Secțiunea " <<  string(1, c) << " rostește " << voice_commands[c-'A'] << endl << endl << endl << endl << endl;
            }
        
        }
    }
    else
    {
        logFile << log_time() << "Error: there was a problem to openeing the section options file for TTS";
    }
}

void initialize_section_options_file_EN()
{
    ofstream SectionOptionsFileEN("../TextToSpeach/EN/option1.txt", ios_base::trunc);
    if (SectionOptionsFileEN.is_open())
    {
        SectionOptionsFileEN << "You selected the Route Follower option. You must select a route from the following:" << endl << endl;
        
        for(char c = 'A'; c <= 'C'; c++)
        {   
            vector<string> routes;
            switch (c) 
            {
                case 'A':
                    routes = RouteRegistration::get_section_A_routes();
                    break;
                case 'B':
                    routes = RouteRegistration::get_section_B_routes();
                    break;
                case 'C':
                    routes = RouteRegistration::get_section_C_routes();
                    break;
                default:
                    /* This should be unreachable */
                    break;
            }
        
            if(0 < routes.size())
            {
                SectionOptionsFileEN << "Section " <<  string(1, c) << endl << endl << endl;
                SectionOptionsFileEN << "   This section comprises the following rooms: " << endl;

                for(int i = 1; i <= routes.size(); i++)
                {
                    SectionOptionsFileEN << "       " << RouteRegistration::get_route_name_from_path(routes[i-1]) << "." << endl << endl << endl;
                }

                SectionOptionsFileEN << "   for the Section " <<  string(1, c) << " say " << voice_commands[c-'A'] << endl << endl << endl << endl << endl;
            }
        
        }
    }
    else
    {
        logFile << log_time() << "Error: there was a problem to openeing the section options file for TTS";
    }
}

void initialize_route_display_files_RO()
{

    for(char c = 'A'; c <= 'C'; c++)
    {   
        vector<string> routes;
        switch (c) 
        {
            case 'A':
                routes = RouteRegistration::get_section_A_routes();
                break;
            case 'B':
                routes = RouteRegistration::get_section_B_routes();
                break;
            case 'C':
                routes = RouteRegistration::get_section_C_routes();
                break;
            default:
                /* This should be unreachable */
                break;
        }
       

        ofstream RouteOptionsFileRO("../TextToSpeach/RO/section_" + string(1, c) + "_options.txt", ios_base::trunc);
        if (RouteOptionsFileRO.is_open())
        {
            RouteOptionsFileRO << "Acum că ai selectat secțiunea. Trebuie să selectezi o rută dintre următoarele:" << endl << endl;
            
            for(int i = 1; i <= routes.size(); i++)
            {
                RouteOptionsFileRO << "Ruta " << i << ". Către " << RouteRegistration::get_route_name_from_path(routes[i-1]) << "." << endl << endl << endl;
                RouteOptionsFileRO << "pentru "<< RouteRegistration::get_route_name_from_path(routes[i-1]) << " rostește " << voice_commands[i-1] << endl << endl << endl << endl << endl;
            }
        }
        else
        {
            logFile << log_time() << "Error: there was a problem to openeing the route options file for TTS";
        }
    }
}

void initialize_route_display_files_EN()
{
    for(char c = 'A'; c <= 'C'; c++)
    {   
        vector<string> routes;
        switch (c) 
        {
            case 'A':
                routes = RouteRegistration::get_section_A_routes();
                break;
            case 'B':
                routes = RouteRegistration::get_section_B_routes();
                break;
            case 'C':
                routes = RouteRegistration::get_section_C_routes();
                break;
            default:
                /* This should be unreachable */
                break;
        }
       

        ofstream RouteOptionsFileEN("../TextToSpeach/EN/section_" + string(1, c) + "_options.txt", ios_base::trunc);
        if (RouteOptionsFileEN.is_open())
        {
            RouteOptionsFileEN << "Now that you selected the building section. You must select a route from the following: " << endl << endl;
            
            for(int i = 1; i <= routes.size(); i++)
            {
                RouteOptionsFileEN << "Route " << i << ". To the " << RouteRegistration::get_route_name_from_path(routes[i-1]) << "." << endl << endl << endl;
                RouteOptionsFileEN << "for the "<< RouteRegistration::get_route_name_from_path(routes[i-1]) << " say " << voice_commands[i-1] << endl << endl << endl << endl << endl;
            }
        }
        else
        {
            logFile << log_time() << "Error: there was a problem to openeing the route options file for TTS";
        }
    }
}
