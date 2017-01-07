//
//  MPEWaveguideSynth.cpp
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-07.
//
//

#include "MPEWaveguideSynth.h"
#include "MPEWaveguideVoice.h"

void MPEWaveguideSynth::renderNextSubBlock (AudioBuffer<float>& buffer, int startSample, int numSamples)
{
    for (int i = voices.size(); --i >= 0;)
    {
        MPEWaveguideVoice* voice = static_cast<MPEWaveguideVoice*>(voices.getUnchecked (i));
        
        if (voice->isActive())
            voice->renderNextBlock (buffer,verbBuffer, startSample, numSamples);
    }
    
    for (int sample = 0; sample < numSamples; ++sample){
        
            verbBuffer[startSample+sample]=reverb.process(buffer.getSample(0, startSample+sample));
    }

};
