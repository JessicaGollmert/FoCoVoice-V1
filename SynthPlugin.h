//
//  SynthPlugin.h
//  TestSynthAU
//
//  Software Synthesizer template project for UWE AMT/CMT students.
//

#ifndef __SynthPlugin_h__
#define __SynthPlugin_h__

#include "PluginProcessor.h"
#include "SynthExtra.h"

//===================================================================================
/** An example STK-voice, based on a sine wave generator                           */
class MyVoice : public Voice
{
public:
    void onStartNote (const int pitch, const float velocity);
    bool onStopNote ();
    
//    void onPitchWheel (const int value);
//    void onControlChange (const int controller, const int value);
    
    bool process (float** outputBuffer, int numChannels, int numSamples);
    
private:    
    float fLevel;
    float fFrequency;
    
    float fVowelGain [2];
    float fConsonantGain = 1.0f;

    int iPitch;
    
    stk::VoicForm vowels[2];
    
    int paramIndex[2] = {1, 2};

    Envelope consonantFade;
    
    Envelope vowelFade1;
    Envelope vowelFade2;
    
    stk::DelayL delay1;
    stk::DelayL delay2;
    
    Buffer consonant;
    
    LPF lowPass;
    
    float pointA = 0.0f;
    float pointB = 0.0f;
    float pointC = 0.0f;
};

class MySynth : public Synth
{
public:
    MySynth() : Synth() {
        initialise();
    }
    
    void initialise();
    void postProcess(float** outputBuffer, int numChannels, int numSamples);

private:
    // Insert synthesizer variables here
};

#endif
