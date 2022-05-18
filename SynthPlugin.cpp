//
//  SynthPlugin.cpp
//  TestSynthAU
//
//  Software Synthesizer template project for UWE AMT/CMT students.
//

#include "SynthPlugin.h"

////////////////////////////////////////////////////////////////////////////
// SYNTH - represents the whole synthesiser
////////////////////////////////////////////////////////////////////////////

// Called to create the synthesizer (use to point JUCE to your synthesizer)
Synth* JUCE_CALLTYPE createSynth() {
    
    std::string filePath = " COPY FILE PATH HERE ";     // CHANGE FILE PATH //
    
    stk::ADSR::setRawwavePath(filePath);
    
    return new MySynth();
}

// Called when the synthesiser is first created
void MySynth::initialise()
{
    // Initialise synthesiser variables here
}

// Used to apply any additional audio processing to the synthesisers' combined output
// (when called, outputBuffer contains all the voices' audio)
void MySynth::postProcess(float** outputBuffer, int numChannels, int numSamples)
{
    // Use to add global effects, etc.
    float fDry0, fDry1;
    float* pfOutBuffer0 = outputBuffer[0];
    float* pfOutBuffer1 = outputBuffer[1];
    
    while(numSamples--)
    {
        fDry0 = *pfOutBuffer0;
        fDry1 = *pfOutBuffer1;
        
        // Add your global effect processing here
        
        *pfOutBuffer0++ = fDry0;
        *pfOutBuffer1++ = fDry1;
    }
}

////////////////////////////////////////////////////////////////////////////
// VOICE - represents a single note in the synthesiser
////////////////////////////////////////////////////////////////////////////

// Called to create the voice (use to point JUCE to your voice)
Voice* JUCE_CALLTYPE createVoice() {
    return new MyVoice();
}

// Triggered when a note is started (use to initialise / prepare note)
void MyVoice::onStartNote (const int pitch, const float velocity)
{
    iPitch = pitch;
    // convert note number to fundamental frequency (Hz)
    fFrequency = 440.f * pow (2.f, (pitch - 69.f) / 12.f);
     
    fLevel = velocity;

    if (getParameter(0) == 0)
    {
        consonant.openResource("1.Consonants/B - Build.wav");
        consonant.setBaseFrequency(1);
    }
    else if (getParameter(0) == 1)
    {
        consonant.openResource("1.Consonants/D - Dance2.wav");
        consonant.setBaseFrequency(1);
    }
    else if (getParameter(0) == 2)
    {
        consonant.openResource("1.Consonants/F - Fish.wav");
        consonant.setBaseFrequency(1);
        fConsonantGain = 0.5f;
    }
    else if (getParameter(0) == 3)
    {
        consonant.openResource("1.Consonants/P - Pause.wav");
        consonant.setBaseFrequency(1);
    }
    else if (getParameter(0) == 4)
    {
        consonant.openResource("1.Consonants/T - Tin.wav");
        consonant.setBaseFrequency(1);
        fConsonantGain = 0.5f;
    }
    else if (getParameter(0) == 5)
    {
        consonant.openResource("1.Consonants/V - View .wav");
        consonant.setBaseFrequency(1);
        fConsonantGain = 0.5f;
    }
    else if (getParameter(0) == 6)
    {
        consonant.openResource("1.Consonants/Z - ZigZag4.wav");
        consonant.setBaseFrequency(1);
    }
    
    
    for (int i = 0; i <= 1; i++)
    {
        vowels[i].clear();
        vowels[i].setFrequency(fFrequency);
        if (getParameter(paramIndex[i]) == 0) {
            vowels[i].setPhoneme("eee");
            fVowelGain[i] = 2.5f;
        }
        else if (getParameter(paramIndex[i]) == 1) {
            vowels[i].setPhoneme("ihh");
            fVowelGain[i] = 0.8f;
        }
        else if (getParameter(paramIndex[i]) == 2) {
            vowels[i].setPhoneme("aaa");
            fVowelGain[i] = 0.5f;
        }
        else if (getParameter(paramIndex[i]) == 3) {
            vowels[i].setPhoneme("uuu");
            fVowelGain[i] = 2.5f;
        }
    }
    
    float t0 = 0.0f;
    float t1 = consonant.getDuration();
    float t2 = t1 + getParameter(3);
    float t3 = t2 + getParameter(4);
    
    // Fades for Consonant
    consonantFade.set(Envelope::Points (t0, 1) (t1, 0) );
    
    // Fades for Vowels
    vowelFade1.set(Envelope::Points (t0, 0) (t1, 1) (t2, 0) );
    vowelFade2.set(Envelope::Points (t0, 0) (t1, 0) (t2, 1) (t3, 0) );
}

// Triggered when a note is stopped (return false to keep the note alive)
bool MyVoice::onStopNote (){
    return true;
}

// Called to render the note's next buffer of audio (generates the sound)
// (return false to terminate the note)
bool MyVoice::process (float** outputBuffer, int numChannels, int numSamples)
{
    float fMix = 0.0f;
    float* fOutL = outputBuffer[0];
    float* fOutR = outputBuffer[1];
    
    float fFade1 = 0.0f;
    float fFade2 = 0.0f;
    float fFade3 = 0.0f;
    
    setParameter(5, consonantFade.tick());
    setParameter(6, vowelFade1.tick());
    setParameter(7, vowelFade2.tick());
    
    float fCutoff = getParameter(8);
    lowPass.setCutoff(fCutoff);

    while(numSamples--)
    {
        pointA = consonant.tick();
        pointA = lowPass.tick(pointA);
        pointA *= fConsonantGain;
        
        pointB = vowels[0].tick();
        pointB *= fVowelGain[0];

        pointC = vowels[1].tick();
        pointC *= fVowelGain[1];
        
        fFade1 = consonantFade.tick();
        fFade2 = vowelFade1.tick();
        fFade3 = vowelFade2.tick();
        
        pointA *= fFade1;
        pointB *= fFade2;
        pointC *= fFade3;

        fMix = pointA + pointB + pointC;
        
        fMix *= fLevel;

        *fOutL = fMix;
        *fOutR = fMix;

        fOutL++;
        fOutR++;
    }
    
    return true;
};
