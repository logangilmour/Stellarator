//
//  MPEWaveguideVoice.cpp
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-02.
//
//

#include "MPEWaveguideVoice.h"
#include <array>
#include <string>
#include "PluginEditor.h"

void MPEWaveguideVoice::noteStarted()
{
    jassert (currentlyPlayingNote.isValid());
    jassert (currentlyPlayingNote.keyState == MPENote::keyDown
             || currentlyPlayingNote.keyState == MPENote::keyDownAndSustained);
    //level.reset(currentSampleRate, smoothingLengthInSeconds);
    //timbre.reset (currentSampleRate, smoothingLengthInSeconds);
    //frequency.reset (currentSampleRate, smoothingLengthInSeconds);
    
    
    // get data from the current MPENote
    level.setValue (currentlyPlayingNote.pressure.asUnsignedFloat());
    frequency.setValue (currentlyPlayingNote.getFrequencyInHertz());
    timbre.setValue (currentlyPlayingNote.timbre.asUnsignedFloat());

    //angle=0;

    //wave.reset();
    //attenuator.reset();
    //harmonicStretcher.reset();
    static uint32 noteStart = 0;
    noteID = noteStart++;

}

int MPEWaveguideVoice::compareTo(MPEWaveguideVoice* other){
    return other->noteID > noteID ? -1 : other->noteID < noteID ? 1 : 0;
}

void MPEWaveguideVoice::noteStopped (bool allowTailOff)
{
    jassert (currentlyPlayingNote.keyState == MPENote::off);
    level.setValue(0);
    if(!allowTailOff){
        clearCurrentNote();

        angle=0;
    wave.reset();
    attenuator.reset();
    harmonicStretcher.reset();
        if(currentSampleRate>0) {
    level.reset(currentSampleRate, smoothingLengthInSeconds);
    timbre.reset (currentSampleRate, smoothingLengthInSeconds);
    frequency.reset (currentSampleRate, smoothingLengthInSeconds);
        }
    }
    
}

void MPEWaveguideVoice::notePressureChanged()
{
    level.setValue (currentlyPlayingNote.pressure.asUnsignedFloat());
    
}

void MPEWaveguideVoice::notePitchbendChanged()
{
    frequency.setValue (currentlyPlayingNote.getFrequencyInHertz());
}

void MPEWaveguideVoice::noteTimbreChanged()
{
    timbre.setValue (currentlyPlayingNote.timbre.asUnsignedFloat());
}

void MPEWaveguideVoice::noteKeyStateChanged()
{
}

void MPEWaveguideVoice::setCurrentSampleRate (double newRate)
{
    if (currentSampleRate != newRate)
    {
        noteStopped (false);
        currentSampleRate = newRate;
        
        level.reset (currentSampleRate, smoothingLengthInSeconds);
        timbre.reset (currentSampleRate, smoothingLengthInSeconds);
        frequency.reset (currentSampleRate, smoothingLengthInSeconds);
    }
}

float MPEWaveguideVoice::process(float feedback){
    float freq = frequency.getNextValue();
    
    angle+=freq/currentSampleRate*double_Pi;
    
    while(angle>2.0*double_Pi){
        angle-=2.0*double_Pi;
    }
    
    float lev = 1-level.getNextValue();
    
    float wlen = getSampleRate()/freq;
    
    
    float blaster = saturate(1-lev);
    float fc = freq*16/44100;
    attenuator.set(fc);

    harmonicStretcher.set(0.1f);
    
    float out = softClip(harmonicStretcher.process(-attenuator.process(wave.read(wlen))));
    
    float wav =0;
    for(int i=0; i<10;i++){
        wav+=sin(angle*(i+1))/(i+1+(1-blaster*blaster)*i*i*10);
        
    }
    wave.write(softClip(wav*blaster*blaster*0.3+out)-feedback*0.1*saturate((blaster-0.95)*20));

    return out;

}
