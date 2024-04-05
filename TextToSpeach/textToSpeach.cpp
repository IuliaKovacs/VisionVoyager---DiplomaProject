#include <iostream>
#include <string>
#include "textToSpeach.h"


#define MODE_OPTIONS_FILE "\"../TextToSpeach/mode_options.txt\""
#define OPTION1_MESSAGE_FILE "\"../TextToSpeach/option1.txt\""
#define OPTION2_MESSAGE_FILE "\"../TextToSpeach/option2.txt\""
#define HELLO_MESSAGE_FILE "\"../TextToSpeach/hello_message.txt\""
#define VISION_VOYAGER_RO_VOICCE "vv"

using namespace std;


void display_menu_options()
{
    string command = "espeak -v " + string(VISION_VOYAGER_RO_VOICCE) + " -s 150 " + " -f " + string(MODE_OPTIONS_FILE);
    system(command.c_str());
}

void display_option1_message()
{
    string command = "espeak -v " + string(VISION_VOYAGER_RO_VOICCE) + " -s 150 " + " -f " + string(OPTION1_MESSAGE_FILE);
    system(command.c_str());
}

void display_option2_message()
{
    string command = "espeak -v " + string(VISION_VOYAGER_RO_VOICCE) + " -s 150 " + " -f " + string(OPTION2_MESSAGE_FILE);
    system(command.c_str());
}

void display_hello_message()
{
    string command = "espeak -v " + string(VISION_VOYAGER_RO_VOICCE) + " -s 150 " + " -f " + string(HELLO_MESSAGE_FILE);
    system(command.c_str());
}

