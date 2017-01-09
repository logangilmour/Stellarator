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
    
    // get data from the current MPENote
    level.setValue (currentlyPlayingNote.pressure.asUnsignedFloat());
    frequency.setValue (currentlyPlayingNote.getFrequencyInHertz());
    timbre.setValue (currentlyPlayingNote.timbre.asUnsignedFloat());
    float vel = currentlyPlayingNote.noteOnVelocity.asUnsignedFloat();
    vel = vel*vel;
    wave.pluck(vel);
    Logger::outputDebugString ("Midi played "+std::to_string(currentlyPlayingNote.getFrequencyInHertz()));
    
}

void MPEWaveguideVoice::noteStopped (bool allowTailOff)
{
    jassert (currentlyPlayingNote.keyState == MPENote::off);
    level.setValue(0);
    if(!allowTailOff){
    clearCurrentNote();
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
    
    float lev = 1-level.getNextValue();
    float wlen = getSampleRate()/freq;
    
    float len = 256/freq;
    
    len = pow(len,0.5);
    
    float blaster = saturate(1-lev);
    float fc = freq*16/44100;
    attenuator.set(fc);
    
    
    harmonicStretcher.set(0.2f);
    
    float out = softClip(harmonicStretcher.process(-attenuator.process(wave.read(wlen))));
    
    
    
    
    
    wave.write(out*(pow(blaster,5)*0.05f*len+1.f)-feedback*0.01*saturate((blaster-0.95)*20));
    return out;

}
