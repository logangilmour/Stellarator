//
//  VarDelay.h
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-04.
//
//

#ifndef VarDelay_h
#define VarDelay_h

#include "../JuceLibraryCode/JuceHeader.h"
#include <array>

class VarDelay{
public:
    float read(float delay);
    void write(float value);
    void pluck(float mag);
    void reset();
private:
    Random random;
    
    static const int Len = 4096;
    static const int Mask = Len-1;
    std::array<float,Len> buffer = {};
    int sample = 0;
    
    
};

#endif /* VarDelay_h */
