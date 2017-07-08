//
//  Wavetable.hpp
//  Stellarator
//
//  Created by Logan Gilmour on 2017-05-28.
//
//

#ifndef Wavetable_h
#define Wavetable_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "DspUtil.h"
#include <array>

class Wavetable{
public:
    Wavetable();
    void reset();
    float process(float level, float freq);
    static int mipOffset(int mip);
    static int mipSize(int mip);
    static float freqToMip(float freq);
private:
    float sample(int mip, int level);
    double angle=0;
    float sampleRate = 44100;
    static const int Levels = 8;
    static const int Timbres = 8;
    static const int mips = 13;
    static const int size =(1<<mips)-1;
    static bool initialized;
    static std::array<std::array<float,size>,Levels> buffer;
};

#endif /* Wavetable_h */
