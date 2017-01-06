//
//  MPEWaveguideVoice.cpp
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-02.
//
//

#include "MPEWaveguideVoice.h"

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
    verb.reset();
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

void MPEWaveguideVoice::renderNextBlock (AudioBuffer<float>& outputBuffer,
                      int startSample,
                      int numSamples)
{
    
    
    
    
    
    
    
    
    for (int sample = 0; sample < numSamples; ++sample){
        float freq = frequency.getNextValue();
        float lev = 1-level.getNextValue();
        float wlen = getSampleRate()/freq;
        

        
        float damper = saturate((lev-0.9f)/0.1f);
        float blaster = saturate(((1-lev)-0.5f)*2.0);
        
        attenuator.set(0.98-damper*0.5);
        
        
        harmonicStretcher.set(0.1f);

        float out = softClip(harmonicStretcher.process(-attenuator.process(wave.read(wlen))));

        
        float reverberation = verb.process(out);
                
        
        
        wave.write(out*(blaster*0.05f+1.f)+reverberation*0.05f);
        
        for (int i = outputBuffer.getNumChannels(); --i >= 0;){
            outputBuffer.addSample (i, startSample+sample, out*0.5f+reverberation*0.5f);
        }
    }
    
    
    
    
}
