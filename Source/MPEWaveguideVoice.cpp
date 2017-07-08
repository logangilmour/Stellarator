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
    //wave.pluck(currentlyPlayingNote.noteOnVelocity.asUnsignedFloat());
    int len = (int)currentSampleRate/currentlyPlayingNote.getFrequencyInHertz();
    int olen = len;
    float count = std::log2(olen);
    Logger::outputDebugString ("Freq "+std::to_string(len)+", "+std::to_string(count));//+": "+std::to_string(voice->vol));

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
    float freq = frequency.getNextValue();
    
    angle+=freq/currentSampleRate*double_Pi;
    
    while(angle>2.0*double_Pi){
        angle-=2.0*double_Pi;
    }
    
    float lev = 1-level.getNextValue();
    
    float wlen = getSampleRate()/(freq);
    
    
    float blaster = saturate(1-lev);
    float fc = freq*32/44100;
    attenuator.set(fc);

    harmonicStretcher.set(0.1f);
    
    float out = softClip(harmonicStretcher.process(-attenuator.process(wave.read(wlen))));
    
    float wav = wavetable.process(saturate(lev),freq);
    
    //float wav =0;
    //for(int i=0; i<50;i++){
    //    wav+=sin(angle*(i+1))/(i+1+(1-blaster*blaster)*i*10);
    //}
    wave.write(softClip(wav*blaster*blaster*2+out)-feedback*0.1*saturate((blaster-0.95)*20));
    volume.set(0.1);
    float curVol = fabs(out);
    vol = volume.process(curVol*curVol);
    if(currentlyPlayingNote.keyState == MPENote::off && vol<0.0000001){
        noteStopped(false);
    }
    //return softClip(wav*blaster);
    return wav;
    return out;
    

}
