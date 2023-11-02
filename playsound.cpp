#include <iostream>
#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include <vector>

#define SAMPLE_RATE (8000)
#define NUM_SECONDS (2)
#define TABLE_SIZE (8000)

const float amplitude = 0.5;
std::vector<double> freq{1209.0,1336.0,1477.0,1633.0,679.0,770.0,852.0,941.0};

// den skal kunne tage en besked og omdanne til binære tal, og sende det videre vha lyd. 
// fx et a bliver omdannet til 0000 0001.

typedef struct
{
    float sine1[TABLE_SIZE];
    float sine2[TABLE_SIZE];
    unsigned int phase;
    char user_input;
} paTestData;

static void checkErr(PaError err){
    if(err != paNoError){
        std::cout << "PortAudio error " << Pa_GetErrorText(err) << std::endl;
        exit(EXIT_FAILURE);
    }
}

static int audioCallback(const void* inputBuffer, void* outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void* userData)
{
    paTestData* data = (paTestData*)userData;
    float* out = (float*)outputBuffer;  

    (void)timeInfo;
    (void)statusFlags;
    (void)inputBuffer;

    static unsigned long i = 0;
    for( i = 0; i < framesPerBuffer; i++){
        switch(data->user_input){
            case '1':
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

            case '2':
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

            case '3':
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

            case 'A': case 'a':
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

            case '4':
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

            case '5':
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

            case '6':
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

            case 'B': case 'b':
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

            case '7':
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

            case '8':
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

            case '9': 
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

            case 'C': case 'c':
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

             case '*': 
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

             case '0': 
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

             case '#': 
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

            case 'D': 
            *out++ = data->sine1[data->phase] + data->sine2[data->phase];
            data->phase +=  1;
            if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE; //fjerner skrætten
            break;

            default : 
            std::cout << "Unrecognized input" << std::endl;
        }
    }

    return paContinue;
}

int main(void)
{
    
    char user_input;

    while(1){
    std::cout << "Enter a DTMF key (or q to quit): ";
    std::cin >> user_input;

    if(user_input == 'q'){
        break;
    }
   

    PaStreamParameters outputParameters;
    PaStream* stream;
    PaError err;
    paTestData data;

    //Sets default DTMF tone to 1
    data.user_input = '1';


    printf("PortAudio Test: Mixing two sine waves. SR = %d\n", SAMPLE_RATE);

    static unsigned int i = 0;

    // DTMF tone generation
    switch(user_input){
        case '1':
        //DTMF tone 1
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[4] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[0] * i / SAMPLE_RATE);
        }
        break;
        
        case '2':
        //DTMF tone 2
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[4] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[1] * i / SAMPLE_RATE);
        }
        break;

        case '3':
        //DTMF tone 3
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[4] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[2] * i / SAMPLE_RATE);
        }
        break;

        case 'A': case 'a':
        //DTMF tone A
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[4] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[3] * i / SAMPLE_RATE);
        }
        break;

        case '4':
        //DTMF tone 3
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[5] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[0] * i / SAMPLE_RATE);
        }
        break;

        case '5':
        //DTMF tone 3
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[5] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[1] * i / SAMPLE_RATE);
        }
        break;

        case '6':
        //DTMF tone 3
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[5] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[2] * i / SAMPLE_RATE);
        }
        break;

        case 'B': case 'b':
        //DTMF tone A
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[5] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[3] * i / SAMPLE_RATE);
        }
        break;

        case '7':
        //DTMF tone 3
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[6] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[0] * i / SAMPLE_RATE);
        }
        break;

        case '8':
        //DTMF tone 3
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[6] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[1] * i / SAMPLE_RATE);
        }
        break;

        case '9':
        //DTMF tone 3
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[6] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[2] * i / SAMPLE_RATE);
        }
        break;

        case 'C': case 'c':
        //DTMF tone A
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[6] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[3] * i / SAMPLE_RATE);
        }
        break;

        case '*':
        //DTMF tone 3
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[7] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[0] * i / SAMPLE_RATE);
        }
        break;

        case '0':
        //DTMF tone 3
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[7] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[1] * i / SAMPLE_RATE);
        }
        break;

        case '#':
        //DTMF tone 3
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[7] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[2] * i / SAMPLE_RATE);
        }
        break;

        case 'D': case 'd':
        //DTMF tone A
        for (i = 0; i < TABLE_SIZE; i++){
        data.sine1[i] = amplitude * sin(2 * M_PI * freq[7] * i / SAMPLE_RATE);
        data.sine2[i] = amplitude * sin(2 * M_PI * freq[3] * i / SAMPLE_RATE);
        }
        break;

        default : 
        std::cout << "Unrecognized input" << std::endl;
        
    }

    data.phase = 0;

    err = Pa_Initialize();
    checkErr(err);

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice){
        fprintf(stderr, "Error: No default output device.\n");
        exit(EXIT_FAILURE);
    }
    
    outputParameters.channelCount = 1;        // Mono output
    outputParameters.sampleFormat = paFloat32; // 32-bit floating-point output
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
        &stream,
        NULL,                 // No input
        &outputParameters,
        SAMPLE_RATE,
        paFramesPerBufferUnspecified,
        paClipOff,             // Don't clip out-of-range samples
        audioCallback,
        &data);
    checkErr(err);

    //afspiller lyden?
    err = Pa_StartStream(stream);
    checkErr(err);

    
    printf("Playing for %d seconds.\n", NUM_SECONDS);
    Pa_Sleep(NUM_SECONDS * 1000);
   
    err = Pa_StopStream(stream);
    checkErr(err);

    err = Pa_CloseStream(stream);
    checkErr(err);

    Pa_Terminate();
    printf("Test finished.\n");

        std::cout << "sine2[" << i << "] = " << data.sine2[i] << std::endl;
        std::cout << "sine1[" << i << "] = " << data.sine1[i] << std::endl;

    }
    return 0;
}
