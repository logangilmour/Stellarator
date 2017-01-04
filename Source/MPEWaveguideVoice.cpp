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
    wave1.Pluck(vel);
    
    
}

void MPEWaveguideVoice::noteStopped (bool allowTailOff)
{
    jassert (currentlyPlayingNote.keyState == MPENote::off);
    
    
    clearCurrentNote();
    wave1.Clear();
    
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
    
    
    
    
    
    
    float c = 0.1f;//screenY;
    
    
    for (int sample = 0; sample < numSamples; ++sample){
        float freq = frequency.getNextValue();
        float lev = 1-level.getNextValue();
        //t = oldT;
        float wlen = getSampleRate()/freq;
        
        
        
        
        
        //float ii = wave[waveR]*fw+wave[(waveR+1)%blen]*(1-fw);
        
        float ii = wave1.Read(wlen);
        
        float damper = fmin(fmax((lev-0.9f)/0.1f,0),1);
        float blaster = fmin(fmax(((1-lev)-0.5f)/0.5f,0),1);
        
        
        oldw = (ii*(0.98f-damper*0.51f)+(oldw*(0.019f+damper*0.5f)));
        
        
        
        
        float in = -oldw;
        
        float out = 0;
        int amt = allcount;
        
        for(int i=0; i<amt; i++){
            float con = c+(float)i/amt*0.1f;
            allnet[i] = -con*in+oldx+ con*allnet[i];
            out+=allnet[i]/amt;
        }
        
        
        oldx = in;
        
        
        float output = verb.processSample(out);
        
        float bell = SoftClip(out+output*0.05f);
        
        blaster = fmax(0,fmin(1,blaster));
        
        
        
        wave1.Write(bell*(blaster*0.05f+1.f));
        
        for (int i = outputBuffer.getNumChannels(); --i >= 0;)
            outputBuffer.addSample (i, startSample+sample, bell*0.5f+output*0.5f);
    }
    
    
    
    
}
