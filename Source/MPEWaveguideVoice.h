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
#include "FDNReverb.h"
#include "VarDelay.h"


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
                          int numSamples) override;
    
    
    
    static float SoftClip(float v){
        float a = 0.04f;
        float b = 0.08f;
        float G = 1.01f;
        return (exp(v*(a+G))-exp(v*(b-G)))/(exp(v)+exp(-v));
        
    }
    
    
private:
    Random random;
    
    LinearSmoothedValue<double> level, timbre, frequency;
    
    float weird = 0;
    float oldw = 0;
    float oldx = 0;
    static const int allcount = 1;
    float allnet[allcount] = {};
    bool left = false;
    int waveW = 0;
    float oldT = 0;

    
    
   
    
    
    
    FDNReverb verb;
    VarDelay wave1;
    
    const double maxLevel = 0.05f;
    const double maxLevelDb = 31.0f;
    const double smoothingLengthInSeconds = 0.01;
};

#endif /* MPEWaveguideVoice_h */
