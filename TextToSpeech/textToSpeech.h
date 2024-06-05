#ifndef TEXT_TO_SPEACH
#define TEXT_TO_SPEACH

#include "../ApplicationModule/application_utils.h"

enum class Language 
{
    RO,
    EN,
    UNKNOWN
};

class TextToSpeech
{   
    static Language language;
    static vector<string> voice_commands;
    static void initialize_section_options_file_RO();
    static void initialize_section_options_file_EN();
    static void initialize_route_display_files_RO();
    static void initialize_route_display_files_EN();

public:

    static Language get_language();
    static void inititalize_language();
    static void set_language_RO();
    static void set_language_EN();
    static void display_menu_options();
    static void display_option1_message();
    static void display_option2_message();
    static void display_hello_message();
    static void display_section_A_options_message();
    static void display_section_B_options_message();
    static void display_section_C_options_message();
    static void display_repeat_message();
    static void initialize_route_display_files();
    static void display_custom_message(string message);
    /* Start of Guiding Moves area */
    static void display_turn_right();
    static void display_turn_left();
    static void display_step_right();
    static void display_step_left();
    static void display_go_forward();
    static void display_go_backward();
    static void display_stop();
    static void display_destination();
    /* End of Guiding Moves area */
};

#endif //TEXT_TO_SPEACH