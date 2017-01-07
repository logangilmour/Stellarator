//
//  MPEWaveguideSynth.h
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-07.
//
//

#ifndef MPEWaveguideSynth_h
#define MPEWaveguideSynth_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "FDNReverb.h"
#include <array>


class MPEWaveguideSynth : public MPESynthesiser{
protected:
    void renderNextSubBlock (AudioBuffer<float>& outputAudio,
                                     int startSample,
                                     int numSamples) override;
    
private:
    FDNReverb reverb;
    std::array<float,512> verbBuffer = {};
};

#endif /* MPEWaveguideSynth_h */