//
//  MPEWaveguideSynth.cpp
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-07.
//
//

#include "MPEWaveguideSynth.h"
#include "MPEWaveguideVoice.h"
#include "DspUtil.h"

void MPEWaveguideSynth::renderNextSubBlock (AudioBuffer<float>& buffer, int startSample, int numSamples)
{
/*
    for (int i = voices.size(); --i >= 0;)
    {
        MPEWaveguideVoice* voice = static_cast<MPEWaveguideVoice*>(voices.getUnchecked (i));
        
        if (voice->isActive())
            voice->renderNextBlock (buffer,verbBuffer, startSample, numSamples);
    }
    
    for (int sample = 0; sample < numSamples; ++sample){
        
            verbBuffer[startSample+sample]=reverb.process(buffer.getSample(0, startSample+sample));
        
    }
 */
    for(int sample = 0; sample<numSamples; ++sample)
    {
        float total = 0;
        for (int i = voices.size(); --i >= 0;)
        {
            MPEWaveguideVoice* voice = static_cast<MPEWaveguideVoice*>(voices.getUnchecked (i));
            
            if (voice->isActive()){
                total+=voice->process(reverbFeedback);
            }
            
        }
        total = softClip(total*0.3);
        reverbFeedback=reverb.process(total);
        buffer.addSample(0, sample+startSample,reverbFeedback*5);
    }


    for(int i=1; i<buffer.getNumChannels(); ++i){
        for(int sample = 0; sample<numSamples; ++sample){
        buffer.addSample(i,startSample+sample , buffer.getSample(0,sample+startSample));
        }
    }

};
