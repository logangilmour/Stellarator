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

void MPEWaveguideVoice::setParams(AudioProcessorValueTreeState* state){
    
}

void MPEWaveguideVoice::noteStarted()
{
    jassert (currentlyPlayingNote.isValid());
    jassert (currentlyPlayingNote.keyState == MPENote::keyDown
             || currentlyPlayingNote.keyState == MPENote::keyDownAndSustained);

    level.setValue (currentlyPlayingNote.pressure.asUnsignedFloat());
    frequency.setValue (currentlyPlayingNote.getFrequencyInHertz());
    timbre.setValue (currentlyPlayingNote.timbre.asUnsignedFloat());
 
    //Logger::outputDebugString ("Freq "+std::to_string(mip)+", "+std::to_string(offset));//+": "+std::to_string(voice->vol));

    playing = true;
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
    playing=false;
    if(!allowTailOff){
        clearCurrentNote();
        volume.reset();
        vol=0;
        angle=0;
    wave.reset();
        //wavetable.reset();
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
    if(playing){
        level.setValue (currentlyPlayingNote.pressure.asUnsignedFloat());
    }
}

void MPEWaveguideVoice::notePitchbendChanged()
{
    if(playing){
        frequency.setValue (currentlyPlayingNote.getFrequencyInHertz());
    }
}

void MPEWaveguideVoice::noteTimbreChanged()
{
    if(playing){
        timbre.setValue (currentlyPlayingNote.timbre.asUnsignedFloat());
    }
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
        vol=0;
        level.reset (currentSampleRate, smoothingLengthInSeconds);
        timbre.reset (currentSampleRate, smoothingLengthInSeconds);
        frequency.reset (currentSampleRate, smoothingLengthInSeconds);
    }
}

float MPEWaveguideVoice::process(float feedback){
    float freq = std::fmaxf(1,frequency.getNextValue());
    
    angle+=freq/currentSampleRate*double_Pi*2;
    
    while(angle>2.0*double_Pi){
        angle-=2.0*double_Pi;
    }
    
    float lev = level.getNextValue();
    
    float wlen = getSampleRate()/(freq*2);
    
    float fc = freq*512/44100;
    attenuator.set(fc);

    harmonicStretcher.set(0.08);
    
    float out = softClip(harmonicStretcher.process(-attenuator.process(wave.read(wlen))));
    
    float wav = wavetable.process(saturate(lev),freq);

    float gate = saturate(1-(1-lev-0.95)*20);
    wave.write(softClip((wav+rand.nextFloat()*0.5)*lev*lev+out)-softClip(feedback*0.01)*gate);
    volume.set(0.1);
    float curVol = fabs(out);
    vol = volume.process(curVol*curVol);
    if(currentlyPlayingNote.keyState == MPENote::off && vol<0.0000001){
        noteStopped(false);
    }
    //return softClip(wav*blaster);
    //return softClip(wav*lev);
    return (softClip(out*wav))*0.5;
    

}
