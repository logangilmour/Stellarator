//
//  DspUtil.h
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-04.
//
//

#ifndef DspUtil_h
#define DspUtil_h

#include <Math.h>

static float SoftClip(float v){
    float a = 0.04f;
    float b = 0.08f;
    float G = 1.01f;
    return (exp(v*(a+G))-exp(v*(b-G)))/(exp(v)+exp(-v));
    
}



#endif /* DspUtil_h */
