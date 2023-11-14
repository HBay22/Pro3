#include <iostream>
#include <string>

#include "portaudio.h"

#include <vector>
#include "fftw3.h"
#include <complex>

#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (1024)

typedef struct{
    int testCounter = 0;
    int finished = paContinue;
} paTestData;

static void checkErr(PaError err){
    if(err != paNoError){
        std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        exit(EXIT_FAILURE);
    }
}

void genkendDTMFtoner(double stor, double lille, paTestData *data){ 
    double at = 35; //Tilladt afvigelse
    std::vector<double> freq{1209, 1336, 1477, 1633, 679, 770, 852, 941};

    if(freq[0]-at < stor && freq[0]+at > stor){
        if (freq[4]-at < lille && freq[4]+at > lille){std::cout << "1" << std::endl;}
        if (freq[5]-at < lille && freq[5]+at > lille){std::cout << "4" << std::endl;}
        if (freq[6]-at < lille && freq[6]+at > lille){std::cout << "7" << std::endl;}
        if (freq[7]-at < lille && freq[7]+at > lille){std::cout << "*" << std::endl;}
    }

    if(freq[1]-at < stor && freq[1]+at > stor){
        if (freq[4]-at < lille && freq[4]+at > lille){std::cout << "2" << std::endl;}
        if (freq[5]-at < lille && freq[5]+at > lille){std::cout << "5" << std::endl;}
        if (freq[6]-at < lille && freq[6]+at > lille){std::cout << "8" << std::endl;}
        if (freq[7]-at < lille && freq[7]+at > lille){std::cout << "0" << std::endl;}
    }

    if(freq[2]-at < stor && freq[2]+at > stor){
        if (freq[4]-at < lille && freq[4]+at > lille){std::cout << "3" << std::endl;}
        if (freq[5]-at < lille && freq[5]+at > lille){std::cout << "6" << std::endl;}
        if (freq[6]-at < lille && freq[6]+at > lille){std::cout << "9" << std::endl;}
        if (freq[7]-at < lille && freq[7]+at > lille){std::cout << "#" << std::endl;}
    }

    if(freq[3]-at < stor && freq[3]+at > stor){
        if (freq[4]-at < lille && freq[4]+at > lille){std::cout << "A" << std::endl;}
        if (freq[5]-at < lille && freq[5]+at > lille){std::cout << "B" << std::endl;}
        if (freq[6]-at < lille && freq[6]+at > lille){std::cout << "C" << std::endl;}
        if (freq[7]-at < lille && freq[7]+at > lille){std::cout << "D" << std::endl; data->finished = paComplete;}

    }
}

static int recordCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData)
{
    paTestData *data = (paTestData *)userData; //unødvendigt med "data"

    (void)outputBuffer; /* Prevent unused variable warnings. */ //Disse skal ikke retunere noget. Vi bruger dem ikke. 
    (void)timeInfo;
    (void)statusFlags;
    (void)userData;

/*En pointer til addressen for inputbufferen (den optagne lyd) i formatet en double.*/
    double *in = (double*)inputBuffer; 

/*FFT*/
    fftw_complex *out;
    out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * (FRAMES_PER_BUFFER / 2 + 1)); //allocere memory til vores output. Rimlig standard kommando
    fftw_plan plan = fftw_plan_dft_r2c_1d(FRAMES_PER_BUFFER, in, out, FFTW_ESTIMATE); //laver en plan, vi bruger estimate siden patient og mesaure ikke virker lige pt.
    fftw_execute(plan); //gør fft'en. Nu er out fyldt med data som er transformet af fft'en

/*Find de to største frekvenser*/
    int storsteAmplitude = -1;
    int andenAmplitude = -1;

    for(int i = 0; i < FRAMES_PER_BUFFER/2 + 1; i++){ //Kun igennem halvdelen (/2) pga spejlning i resultatet.
        if(out[i][0] * out[i][0] + out[i][1] * out[i][1] > out[storsteAmplitude][0] * out[storsteAmplitude][0] + out[storsteAmplitude][1] * out[storsteAmplitude][1]){ //magnitude, hvor kvadratroden er "sparet" væk
            andenAmplitude = storsteAmplitude;
            storsteAmplitude = i;
        }
    }

    double storsteFrekvens = (double)storsteAmplitude * SAMPLE_RATE / FRAMES_PER_BUFFER/2; //Vi dividere med 2 pga. spejlning af resultatet.
    double andenFrekvens = (double)andenAmplitude * SAMPLE_RATE / FRAMES_PER_BUFFER/2;

/*Printer*/
    // std::cout << "Nummer: " << data->testCounter << std::endl;
    genkendDTMFtoner(storsteFrekvens, andenFrekvens, data);
    // std::cout << "storsteFrekvens: " << storsteFrekvens << std::endl;
    // std::cout << "andenFrekvens:    " << andenFrekvens << std::endl;
    // std::cout << std::endl;
    data->testCounter++;
    
/*Stop FFT*/    
    fftw_destroy_plan(plan);
    fftw_free(out);
    fftw_cleanup();

/*Retunerer finished, som enten retunere "paContinue" (forsæt optagelse) eller "paComplete" (færdig med optagelse).*/
    return data->finished;    
}



/*MAIN:*/
int main(void)
{
    PaStreamParameters inputParameters;

    PaStream *stream;
    PaError err = paNoError;
    paTestData data;

    err = Pa_Initialize();
    checkErr(err);

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice){
        fprintf(stderr, "Error: No default input device.\n");
    }
    checkErr(err);

    inputParameters.channelCount = 1; /* Mono input */
    inputParameters.sampleFormat = paFloat32; // Sample format er float
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream( 
        &stream,
        &inputParameters,
        NULL, /* &outputParameters, */
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff, /* we won't output out of range samples so don't bother clipping them */
        recordCallback,
        &data);
    checkErr(err);

    err = Pa_StartStream(stream);
    checkErr(err);

    std::cout << "\n=== Now recording!! ===\n" << std::endl;

    while ((err = Pa_IsStreamActive(stream)) == 1){ //Optager intil array er udfyldt
        Pa_Sleep(10);
    }
    checkErr(err);

    err = Pa_CloseStream(stream);
    checkErr(err);

    std::cout << "\n === Done recording!! ===" << std::endl;

    Pa_Terminate();

    return err;
}
