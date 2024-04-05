#ifndef TEXT_TO_SPEACH
#define TEXT_TO_SPEACH

enum class Language 
{
    RO,
    EN
};

extern Language language;

void set_language_RO();
void set_language_EN();
void display_menu_options();
void display_option1_message();
void display_option2_message();
void display_hello_message();

#endif //TEXT_TO_SPEACH