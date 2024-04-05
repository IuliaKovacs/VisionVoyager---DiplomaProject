#include <iostream>
#include <string>
#include "textToSpeach.h"

#define VISION_VOYAGER_RO_VOICE "vv"
#define VISION_VOYAGER_EN_VOICE "mb-en1"

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

#define CHOOSE_LANGUAGE(lang) ((lang == Language::RO) ? VISION_VOYAGER_RO_VOICE : VISION_VOYAGER_EN_VOICE)



using namespace std;

/* Convention that language should be English by default */
Language language = Language::EN;


void set_language_RO(){
    language = Language::RO;
}

void set_language_EN(){
    language = Language::EN;
}

void display_menu_options()
{   
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

void display_hello_message()
{
    string command = "espeak -v " + string(CHOOSE_LANGUAGE(language)) + " -s 150 " + " -f " + string(HELLO_MESSAGE_FILE(language));
    system(command.c_str());
}

