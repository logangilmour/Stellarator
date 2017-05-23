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
#include "LowpassFilter.h"
#include "AllpassFilter.h"
#include <array>
#include <vector>

class FDNReverb {
public:
    
    FDNReverb();

    float process(float sample);
    void reset();
    
private:
    static const int delayCount = 16;
    static const int len = 97;
    
    std::array<bool,16> printed = {};
    
    std::array<AllpassFilter,delayCount> allpass = {};
    
    std::array<int,24> primes = {2,3,5,7,11,13,17,19,23,29,31,37,41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89};
    
    std::array<std::vector<float>, delayCount> delays;
    std::array<int,delayCount> writers = {};
    std::array<LowpassFilter,delayCount> attenuators = {};
};

#endif /* FDNReverb_h */
