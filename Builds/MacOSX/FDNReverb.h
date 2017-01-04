//
//  FDNReverb.h
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-04.
//
//

#ifndef FDNReverb_h
#define FDNReverb_h

#include "DspUtil.h"

class FDNReverb {
public:
    
    FDNReverb();

    float processSample(float sample);
    
private:
    static const int delayCount = 16;
    static const int len = 128;
    
    bool printed[16] = {};
    
    float allpass[delayCount] = {};
    
    int primes[24] = {2,3,5,7,11,13,17,19,23,29,31,37,41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89};
    
    float *delays[delayCount];
    int writers[delayCount] = {};
    float olds[delayCount] = {};
    
};

#endif /* FDNReverb_h */
