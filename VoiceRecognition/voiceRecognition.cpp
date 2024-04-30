
#include "voiceRecognition.h"
#include <pocketsphinx.h>
#include <signal.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <time.h>

#define SOX_COMMAND "sox -q -r %d -c 1 -b 16 -e signed-integer -d -t raw -"
#define RECORDING_DURATION_SECONDS 5
#define SAMPLE_RATE 16000 
#define NUM_CHANNELS  1
#define TEMP_AUDIO_FILE_PATH "../VoiceRecognition/recorded_audio.wav"
#define START_KEYPHRASES_LIST "../VoiceRecognition/start_keyphrase.list"
#define KEYPHRASES_LIST "../VoiceRecognition/keyphrases.list"

using namespace std;
int global_done = 0;
/* @ToDo - reset start_flag in specific cases */
bool VoiceRecognition::start_flag = false;


static void catch_sig(int signum) 
{
    (void)signum;
    global_done = 1;
}

static FILE * popen_sox(int sample_rate) 
{
    char *soxcmd;
    int len;
    FILE *sox;

    len = snprintf(NULL, 0, SOX_COMMAND, sample_rate);
    if ((soxcmd = (char *)malloc(len + 1)) == NULL)
    {
        logFile << "Error: Failed to allocate string" << endl;
    }
    if (snprintf(soxcmd, len + 1, SOX_COMMAND, sample_rate) != len)
    {
        logFile << "Error: snprintf() failed" << endl;
    }
    if ((sox = popen(soxcmd, "r")) == NULL)
    {
        logFile << "Error: Failed to popen(" << soxcmd << ")" << endl;
    }
    free(soxcmd);

    return sox;
}


void VoiceRecognition::loop_recognition()
{
    ps_decoder_t *decoder;
    ps_endpointer_t *ep;
    FILE *sox;
    short *frame;
    size_t frame_size;

    ps_config_t *config = ps_config_init(NULL);

    ps_config_set_str(config, "kws", START_KEYPHRASES_LIST);
    ps_default_search_args(config);

    decoder = ps_init(config);

    if (decoder == NULL)
    {
         logFile << "Error: PocketSphinx decoder init failed\n" << endl;
    }   

    ps_vad_mode_t vad_mode = PS_VAD_LOOSE;
    ep = ps_endpointer_init(0, 0.0, vad_mode, 0, 0);
    if (ep == NULL)
    {
        logFile << "Error: PocketSphinx endpointer init failed\n" << endl;
    }

    sox = popen_sox(ps_endpointer_sample_rate(ep));
    frame_size = ps_endpointer_frame_size(ep);

    if ((frame = (short *)malloc(frame_size * sizeof(frame[0]))) == NULL)
    {
        logFile << "Error: Failed to allocate frame" << endl;
    }

    if (signal(SIGINT, catch_sig) == SIG_ERR)
    {
        logFile << "Error: Failed to set SIGINT handler" << endl;
    }

    while (!global_done && (false == start_flag)) 
    {
        const int16 *speech;
        int prev_in_speech = ps_endpointer_in_speech(ep);
        size_t len, end_samples;

        if ((len = fread(frame, sizeof(frame[0]), frame_size, sox)) != frame_size) 
        {
            if (len > 0) 
            {
                speech = ps_endpointer_end_stream(ep, frame, frame_size, &end_samples);
            } 
            else 
            {
                break;
            }
        } 
        else 
        {
            speech = ps_endpointer_process(ep, frame);
        }

        if (speech != NULL) 
        {
            const char *hyp;
            if (!prev_in_speech) 
            {
                ps_start_utt(decoder);
            }

            if (ps_process_raw(decoder, speech, frame_size, FALSE, FALSE) < 0)
            {
                logFile << "Error: ps_process_raw() failed\n" << endl; 
            }

            if (!ps_endpointer_in_speech(ep)) 
            {
                ps_end_utt(decoder);
                if ((hyp = ps_get_hyp(decoder, NULL)) != NULL)
                {
                    logFile << "Recognized words: +" << hyp << "+" << endl;
                    if ((strstr("start",hyp) == 0) || (strstr("hello",hyp) == 0)) 
                    {
                        start_flag = true;
                    }
                }    
            }
        }
    }

    free(frame);
    if (pclose(sox) < 0)
    {
        logFile << "Error: Failed to pclose(sox)" << endl;
    }
    ps_endpointer_free(ep);
    ps_free(decoder);
    ps_config_free(config);
}


bool VoiceRecognition::record_audio()
{
    string command = "arecord -d " + to_string(RECORDING_DURATION_SECONDS) +
            " -f S16_LE -r " + to_string(SAMPLE_RATE) +
            " -c " + to_string(NUM_CHANNELS) + " " + TEMP_AUDIO_FILE_PATH;

    logFile << "Recording audio..." << endl;
    int result = system(command.c_str());

    if (result == 0) 
    {
        logFile << "Audio recording complete" << endl;
        return true;
    } 
    else 
    {
        logFile << "Error: recording audio failed" << endl;
        return false;
    }
}


string VoiceRecognition::timed_listening_recognition_for_options()
{
    record_audio();
    string recognized_word;

    ps_decoder_t *decoder;
    ps_config_t *config;
    FILE *fh;
    short *buf;
    size_t len, nsamples;

    if ((fh = fopen(TEMP_AUDIO_FILE_PATH, "rb")) == NULL)
    {
        logFile << "Error: Failed to open " << TEMP_AUDIO_FILE_PATH << endl;
    }

    if (fseek(fh, 0, SEEK_END) < 0)
    {
        logFile << "Error: Unable to find end of input file " << TEMP_AUDIO_FILE_PATH << endl;
    }
    len = ftell(fh);
    rewind(fh);

    config = ps_config_init(NULL);
    ps_config_set_str(config, "kws", KEYPHRASES_LIST);
    ps_default_search_args(config);

    if (ps_config_soundfile(config, fh, TEMP_AUDIO_FILE_PATH) < 0)
    {
        logFile << "Error: Unsupported input file " << TEMP_AUDIO_FILE_PATH << endl;
    }
    if ((decoder = ps_init(config)) == NULL)
    {
        logFile << "Error: PocketSphinx decoder initialization failed\n" << endl;
    }

    len -= ftell(fh);
    if ((buf = (short *)malloc(len)) == NULL)
    {
        logFile << "Error: Unable to allocate " << len << " bytes" << endl;
    }

    nsamples = fread(buf, sizeof(buf[0]), len / sizeof(buf[0]), fh);

    if (nsamples != len / sizeof(buf[0]))
    {
        logFile << "Error: Unable to read samples" << endl;
    }

    if (ps_start_utt(decoder) < 0)
    {
        logFile << "Error: Failed to start processing" << endl;
    }
    if (ps_process_raw(decoder, buf, nsamples, FALSE, TRUE) < 0)
    {
        logFile << "Error: ps_process_raw() failed" << endl;
    }
    if (ps_end_utt(decoder) < 0)
    {
        logFile << "Error: Failed to end processing" << endl;
    }

    if (ps_get_hyp(decoder, NULL) != NULL)
    {
        recognized_word = ps_get_hyp(decoder, NULL);
        if (strstr("ONE", recognized_word.c_str()) == 0)
        {
            recognized_word = "ONE";
        }
        else if (strstr("TWO", recognized_word.c_str()) == 0)
        {
            recognized_word = "TWO";
        }
        else
        {
            recognized_word = "UNKNOWN";
        }
        logFile << "Recognized word: " << recognized_word << endl;
    }

    if (fclose(fh) < 0)
    {
        logFile << "Error: failed to close file " << TEMP_AUDIO_FILE_PATH << endl;
    }
    free(buf);
    ps_free(decoder);
    ps_config_free(config);

    return recognized_word;
}