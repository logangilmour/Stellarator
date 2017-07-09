//
//  DspUtil.h
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-04.
//
//

#ifndef DspUtil_h
#define DspUtil_h

#include <math.h>

static float softClip(float v){
    float a = 0.04f;
    float b = 0.08f;
    float G = 1.01f;
    return (exp(v*(a+G))-exp(v*(b-G)))/(exp(v)+exp(-v));
    
}

static float lerp(float v1, float v2, float ratio){
    return v1*ratio+v2*(1-ratio);
}

static float saturate(float v){
    return fmax(0,fmin(1,v));
}



#endif /* DspUtil_h */
