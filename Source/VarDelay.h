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

class VarDelay{
public:
    float Read(float delay);
    void Write(float value);
    void Pluck(float mag);
    void Clear();
private:
    Random random;
    
    static const int Len = 4096;
    static const int Mask = Len-1;
    float buffer[Len] = {};
    int sample = 0;
    
    
};

#endif /* VarDelay_h */
