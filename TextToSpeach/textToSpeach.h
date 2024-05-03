#ifndef TEXT_TO_SPEACH
#define TEXT_TO_SPEACH

#include "../ApplicationModule/application_utils.h"

enum class Language 
{
    RO,
    EN,
    UNKNOWN
};

extern Language language;

void inititalize_language();
void set_language_RO();
void set_language_EN();
void display_menu_options();
void display_option1_message();
void display_option2_message();
void display_hello_message();
void display_section_A_options_message();
void display_section_B_options_message();
void display_section_C_options_message();
void display_repeat_message();
void initialize_route_display_files();
void initialize_section_options_file_RO();
void initialize_section_options_file_EN();
void initialize_route_display_files_RO();
void initialize_route_display_files_EN();

#endif //TEXT_TO_SPEACH