#include <iostream>
#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include <vector>
#include <fstream>
#include <unistd.h>
#include <string>

#define SAMPLE_RATE (8000)
#define TABLE_SIZE (8000)


typedef struct{
    float dtmf[16][TABLE_SIZE];
    unsigned int dmtfValg;
    unsigned int phase;
} paData;


static int audioCallback(const void* inputBuffer, void* outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void* userData)
{
    paData* data = (paData*)userData;
    float* out = (float*)outputBuffer;

    (void)timeInfo;
    (void)statusFlags;
    (void)inputBuffer;


    //Sammensætter og gør lyden klar til afspildning via *out++
    static unsigned long i;
    for (i = 0; i < framesPerBuffer; i++){

        *out++ = data->dtmf[data->dmtfValg][data->phase];
        if(data->dmtfValg == 16){
            *out++ = 0.0f;
        }
        data->phase +=  1;
        if (data->phase >= TABLE_SIZE) data->phase -= TABLE_SIZE;
    }

    return paContinue;
}

int main(void)
{
    PaStreamParameters outputParameters;
    PaStream* stream;
    paData data;

    // DTMF frekvenser
    float freqRow[4] = {1209,1336,1477,1633};
    float freqcol[4] = {679,770,852,941};
    const float amplitude = 0.5;

    // Udregner DTMF tonerne vha. for loop, og gemmer dem i data.dtmf
    //Udregner dem som 1..2..3..A..4 Osv...
    float sin1, sin2 = 0;
    int j =0;
    for(int l = 0; l < 4; l++){
        for(int k = 0; k < 4; k++){
            for (int i = 0; i < TABLE_SIZE; i++){
                sin1 = amplitude * sin(2 * M_PI * freqRow[k] * i / SAMPLE_RATE);
                sin2 = amplitude * sin(2 * M_PI * freqcol[l] * i / SAMPLE_RATE);
                data.dtmf[j][i] = sin1 + sin2;
            }
            j++;
        }
    }
    
    data.phase = 0;

   Pa_Initialize();
    

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice){
        fprintf(stderr, "Error: No default output device.\n");
        exit(EXIT_FAILURE);
    }
    
    outputParameters.channelCount = 1;        // Mono output
    outputParameters.sampleFormat = paFloat32; // 32-bit floating-point output
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

        Pa_OpenStream(
        &stream,
        NULL,                 // No input
        &outputParameters,
        SAMPLE_RATE,
        paFramesPerBufferUnspecified,
        paClipOff,             // Don't clip out-of-range samples
        audioCallback,
        &data);
   

    Pa_StartStream(stream);
    
    std::vector<int> sequence = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,2,3,3,4,5,6,6,7,5,5,4,12,12,12,11,10,10,1,1,1,2,3,4,5,6,7,8,9,9,9,9};

   
    for(int i = 0; i < sequence.size(); i++){ // looper igennem sekvensen
        data.dmtfValg = sequence[i];
        usleep(20*1000); //hvor lang tid lyden spiller.
    } 
    

    Pa_StopStream(stream);
    

    Pa_CloseStream(stream);
    

    Pa_Terminate();
    return 0;
}

//afspiller nu lyd nemt, men stadig med pops. (ved ikke fix, evt spørg vejleder.)
