#ifndef VOICE_RECOGNITION
#define VOICE_RECOGNITION


#include <pocketsphinx.h>
#include <signal.h>
#include <iostream>
#include <stdlib.h>

enum class RecognitionType
{   
    UNDEFINED,
    TIMED_RECOGNITION,
    LIVE_RECOGNITION
};


class VoiceRecognition
{   
    static bool record_audio();

public:

    static void loop_recognition();
    static void timed_listening_recognition();

};

#endif //VOICE_RECOGNITION