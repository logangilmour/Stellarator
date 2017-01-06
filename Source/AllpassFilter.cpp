//
//  AllpassFilter.cpp
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-06.
//
//

#include "AllpassFilter.h"

float AllpassFilter::process(float sample){
    float newval = buffer+c*(sample-c*buffer);
    
    buffer = sample-c*buffer;
    
    return newval;
    
}
void AllpassFilter::set(float v){
    c = v;
}
void AllpassFilter::reset(){
    buffer = 0;
    c = 0;
}
