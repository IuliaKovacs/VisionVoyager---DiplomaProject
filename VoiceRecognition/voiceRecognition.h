#ifndef VOICE_RECOGNITION
#define VOICE_RECOGNITION

#include "../ApplicationModule/application_utils.h"
#include <pocketsphinx.h>
#include <signal.h>
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
    static void loop_listening_for_stop();
};

#endif //VOICE_RECOGNITION