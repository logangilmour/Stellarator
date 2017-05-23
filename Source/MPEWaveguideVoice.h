//
//  MPEWaveguideVoice.h
//  Stellarator
//
//  Created by Logan Gilmour on 2016-12-30.
//
//

#ifndef MPEWaveguideVoice_h
#define MPEWaveguideVoice_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "DspUtil.h"
#include "VarDelay.h"
#include "LowpassFilter.h"
#include "AllpassFilter.h"


class MPEWaveguideVoice : public MPESynthesiserVoice{
public:
    void noteStarted() override;
    
    void noteStopped (bool allowTailOff) override;
    
    void notePressureChanged() override;
    
    void notePitchbendChanged() override;
    
    void noteTimbreChanged() override;
    
    void noteKeyStateChanged() override;
    
    void setCurrentSampleRate (double newRate) override;
    
    //==============================================================================
    void renderNextBlock (AudioBuffer<float>& outputBuffer,
                          int startSample,
                          int numSamples) override {};
    
    
    float process(float feedback);
    
    
    
    
    
private:
    Random random;
    
    LinearSmoothedValue<double> level, timbre, frequency, tailoff;
    LowpassFilter attenuator;
    AllpassFilter harmonicStretcher;

    VarDelay wave;

    float angle;
    
    const double maxLevel = 0.05f;
    const double maxLevelDb = 31.0f;
    const double smoothingLengthInSeconds = 0.01;
};

#endif /* MPEWaveguideVoice_h */
