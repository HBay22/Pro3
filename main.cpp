#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "portaudio.h"

#include <vector>
#include "fftw3.h"
#include <complex.h>

/* #define rate */
#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (1024)
#define NUM_SECONDS (60)


/* Select sample format. */
#define PA_SAMPLE_TYPE paFloat32        //Slet?
typedef float SAMPLE;                   //Slet?
#define SAMPLE_SILENCE (0.0f)           //slet? måske????


typedef struct
{
    int testCounter = 0;
    int frameIndex; /* Index into sample array. */
    int maxFrameIndex;
    SAMPLE *recordedSamples;            //SLET
} paTestData;

void genkendDTMFtoner(double stor, double lille){
    double at = 35; //Tilladt afvigelse
    std::vector<double> freq{1209, 1336, 1477, 1633, 679, 770, 852, 941};

    if(freq[0]-at < stor && freq[0]+at > stor){
        if (freq[4]-at < lille && freq[4]+at > lille){std::cout << "Tone: 1" << std::endl;}
        if (freq[5]-at < lille && freq[5]+at > lille){std::cout << "Tone: 4" << std::endl;}
        if (freq[6]-at < lille && freq[6]+at > lille){std::cout << "Tone: 7" << std::endl;}
        if (freq[7]-at < lille && freq[7]+at > lille){std::cout << "Tone: *" << std::endl;}
    }

    if(freq[1]-at < stor && freq[1]+at > stor){
        if (freq[4]-at < lille && freq[4]+at > lille){std::cout << "Tone: 2" << std::endl;}
        if (freq[5]-at < lille && freq[5]+at > lille){std::cout << "Tone: 5" << std::endl;}
        if (freq[6]-at < lille && freq[6]+at > lille){std::cout << "Tone: 8" << std::endl;}
        if (freq[7]-at < lille && freq[7]+at > lille){std::cout << "Tone: 0" << std::endl;}
    }

    if(freq[2]-at < stor && freq[2]+at > stor){
        if (freq[4]-at < lille && freq[4]+at > lille){std::cout << "Tone: 3" << std::endl;}
        if (freq[5]-at < lille && freq[5]+at > lille){std::cout << "Tone: 6" << std::endl;}
        if (freq[6]-at < lille && freq[6]+at > lille){std::cout << "Tone: 8" << std::endl;}
        if (freq[7]-at < lille && freq[7]+at > lille){std::cout << "Tone: #" << std::endl;}
    }

    if(freq[3]-at < stor && freq[3]+at > stor){
        if (freq[4]-at < lille && freq[4]+at > lille){std::cout << "Tone: A" << std::endl;}
        if (freq[5]-at < lille && freq[5]+at > lille){std::cout << "Tone: B" << std::endl;}
        if (freq[6]-at < lille && freq[6]+at > lille){std::cout << "Tone: C" << std::endl;}
        if (freq[7]-at < lille && freq[7]+at > lille){std::cout << "Tone: D" << std::endl;}
    }


}

static int recordCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData)
{
    paTestData *data = (paTestData *)userData; //unødvendigt med "data"

    long framesToCalc;
    int finished;
    unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

    (void)outputBuffer; /* Prevent unused variable warnings. */ //Disse skal ikke retunere noget. Vi bruger dem ikke. 
    (void)timeInfo;
    (void)statusFlags;
    (void)userData;

/*Når der mangler færre end framesPerBuffer (1024) værdier at bliver optaget/behandlet, så sætter den framesToCalc = framesLeft,
hvilket betyder, at den blot mangler framesLeft frames, hvorefter optagelsen er slut (finished = paComplete).
Hvis den ikke er færdig, så sætter den bare "framesToCalc" til størrelse på bufferen, som den så kan optage/behandle. */
    if (framesLeft < framesPerBuffer){
        framesToCalc = framesLeft;
        finished = paComplete; 
    }
    else{
        framesToCalc = framesPerBuffer; 
        finished = paContinue;
    }

//En pointer til addressen for inputbufferen (den optagne lyd) i formatet en double.
    double *in = (double*)inputBuffer; 

//FFT kommer her
    fftw_complex *out; //standard fftw commando
    out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * (FRAMES_PER_BUFFER / 2 + 1)); //allocere memory til vores output. Rimlig standard kommando
    fftw_plan plan = fftw_plan_dft_r2c_1d(FRAMES_PER_BUFFER, in, out, FFTW_ESTIMATE); //laver en plan, vi bruger estimate siden patient og mesaure ikke virker lige pt.
    fftw_execute(plan); //gør fft'en. Nu er out fyldt med data som er transformet af fft'en

//Find de to største amplituder
    int storsteAmplitude = -1; //største amplitude
    int andenAmplitude = -1; //anden største amplitude

//Loop'er igennem dem...
    for(int i = 0; i < FRAMES_PER_BUFFER/2 + 1; i++){ //Kun igennem halvdelen (/2) pga spejlning i resultatet.
        if(out[i][0] * out[i][0] + out[i][1] * out[i][1] > out[storsteAmplitude][0] * out[storsteAmplitude][0] + out[storsteAmplitude][1] * out[storsteAmplitude][1]){ //magnitude, hvor kvadratroden er "sparet" væk
            andenAmplitude = storsteAmplitude;
            storsteAmplitude = i;
        } //Tror den fungere bedre, da den ikke break'er, men den forsætter gennem hele datasættet, så den stopper ved første lejlighed ligesom som den anden....
    }

    double storsteFrekvens = (double)storsteAmplitude * SAMPLE_RATE / FRAMES_PER_BUFFER/2; //HVOR DIVIDERE MED TO????
    double andenFrekvens = (double)andenAmplitude * SAMPLE_RATE / FRAMES_PER_BUFFER/2;

 //   std::cout << "Nummer: " << data->testCounter << std::endl;
    genkendDTMFtoner(storsteFrekvens, andenFrekvens);
 //   std::cout << "storsteFrekvens: " << storsteFrekvens << std::endl;
 //   std::cout << "andenFrekvens:    " << andenFrekvens << std::endl;
 //   std::cout << std::endl;
    data->testCounter++;
    
//Stop FFT.
    fftw_destroy_plan(plan);
    fftw_free(out);
    fftw_cleanup();


//Opdaterer "frameIndex" med antallet af frames der er blevet optaget. 
    data->frameIndex += framesToCalc;  //SLET?

//Retunerer finished, som enten retunere "paContinue", hvis den skal forsætte med at optage...
//eller den retunere "paComplete", hvis den er færdig med at optage... 
    return finished;
}


/*******************************************************************/
int main(void);
int main(void)
{
    PaStreamParameters inputParameters,
        outputParameters;
    PaStream *stream;
    PaError err = paNoError;
    paTestData data;
    int i;
    int totalFrames;
    int numBytes;
    SAMPLE max, val;
    double average;

//SLET MÅSKE
    data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */ //MAX
    data.frameIndex = 0; //starter ved frameIndex 0
    numBytes = totalFrames * sizeof(SAMPLE); //numbytes = antallet af samples * størrelsen på en sample.
    data.recordedSamples = (SAMPLE *)malloc(numBytes); /* From now on, recordedSamples is initialised. */ //Allokerer plads i et array på størrelse numBytes
    if (data.recordedSamples == NULL){ //Error tjek
        printf("Could not allocate record array.\n");
        goto done;
    }

    for (i = 0; i < totalFrames; i++){ //Sætter alle værdier til nul i hele array'et            //SLET??
        data.recordedSamples[i] = 0;
    } 
//SLET MÅSKE HER TIL

    err = Pa_Initialize();
    if (err != paNoError)
        goto done;

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice){
        fprintf(stderr, "Error: No default input device.\n");
        goto done;
    }

    inputParameters.channelCount = 1; /* Mono input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE; // Sample format er float
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    /* Record some audio. -------------------------------------------- */
    err = Pa_OpenStream( 
        &stream,
        &inputParameters,
        NULL, /* &outputParameters, */
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff, /* we won't output out of range samples so don't bother clipping them */
        recordCallback,
        &data);
    if (err != paNoError)
        goto done;

    err = Pa_StartStream(stream);
    if (err != paNoError)
        goto done;
    printf("\n=== Now recording!! ===\n");
    fflush(stdout);

    while ((err = Pa_IsStreamActive(stream)) == 1){ //Optager intil array er udfyldt
        Pa_Sleep(500); //Sover i 0,5 sekunder.
    }

    if (err < 0)
        goto done;

    err = Pa_CloseStream(stream);
    if (err != paNoError)
        goto done;

    std::cout << "\n === SLUT ===" << std::endl;
done:
    Pa_Terminate();
    if (err != paNoError)
    {
        fprintf(stderr, "An error occurred while using the portaudio stream\n");
        fprintf(stderr, "Error number: %d\n", err);
        fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
        err = 1; /* Always return 0 or 1, but no other return codes. */
    }
    return err;
}
