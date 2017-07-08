//
//  LowpassFilter.cpp
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-05.
//
//

#include "LowpassFilter.h"
#include <cmath>

float LowpassFilter::process(float sample){
    buffer = sample*(1-k)+buffer*k;
    return buffer;
}
void LowpassFilter::set(float freq){
        k = exp(-2.0 * M_PI * freq);
}
void LowpassFilter::reset(){
    buffer = 0;
    k = 1;
}
