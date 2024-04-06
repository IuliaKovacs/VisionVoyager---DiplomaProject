#ifndef VOICE_RECOGNITION
#define VOICE_RECOGNITION


#include <pocketsphinx.h>
#include <signal.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

enum class RecognitionType
{   
    UNDEFINED,
    TIMED_RECOGNITION,
    LIVE_RECOGNITION
};


class VoiceRecognition
{   
    static bool record_audio();
    static bool start_flag;

public:

    static void loop_recognition();
    static string timed_listening_recognition_for_options();

};

#endif //VOICE_RECOGNITION