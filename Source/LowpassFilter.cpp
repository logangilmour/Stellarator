//
//  LowpassFilter.cpp
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-05.
//
//

#include "LowpassFilter.h"

float LowpassFilter::process(float sample){
    buffer = sample*k+buffer*(1-k);
    return buffer;
}
void LowpassFilter::set(float v){
    k = v;
}
void LowpassFilter::reset(){
    buffer = 0;
    k = 1;
}
