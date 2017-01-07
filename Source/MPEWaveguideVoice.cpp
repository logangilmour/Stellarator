//
//  MPEWaveguideVoice.cpp
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-02.
//
//

#include "MPEWaveguideVoice.h"
#include <array>

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
    
    
}

void MPEWaveguideVoice::noteStopped (bool allowTailOff)
{
    jassert (currentlyPlayingNote.keyState == MPENote::off);
    
    
    clearCurrentNote();
    wave.reset();
    attenuator.reset();
    harmonicStretcher.reset();

    
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

void MPEWaveguideVoice::renderNextBlock (AudioBuffer<float>& outputBuffer, std::array<float,512> feedback,
                      int startSample,
                      int numSamples)
{
    
    
    
    
    
    
    
    
    for (int sample = 0; sample < numSamples; ++sample){
        float freq = frequency.getNextValue();
        float lev = 1-level.getNextValue();
        float wlen = getSampleRate()/freq;
        

        
        float damper = saturate(lev);
        float blaster = saturate(1-lev);
        
        attenuator.set(0.94-damper*0.5);
        
        
        harmonicStretcher.set(0.2f);

        float out = softClip(harmonicStretcher.process(-attenuator.process(wave.read(wlen))));

        
        
        
        
        wave.write(out*(pow(blaster,5)*0.025f+1.f)-feedback[startSample+sample]*0.1);
        
        for (int i = outputBuffer.getNumChannels(); --i >= 0;){
            outputBuffer.addSample (i, startSample+sample, out);
        }
    }
    
    
    
    
}
