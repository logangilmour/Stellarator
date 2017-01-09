//
//  LPF.h
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-05.
//
//

#ifndef LowpassFilter_h
#define LowpassFilter_h

class LowpassFilter{
public:
    float process(float sample);
    void reset();
    void set(float freq);
private:
    float buffer = 0;
    float k = 0.0f;
};

#endif /* LPF_h */
