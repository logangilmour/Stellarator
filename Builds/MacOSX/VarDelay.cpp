//
//  VarDelay.cpp
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-04.
//
//

#include "VarDelay.h"

    float VarDelay::Read(float delay){
        
        
        float iwd = 0;
        
        float fw = modff(sample+Len-(delay-1), &iwd);
        int iw = (int)iwd;
        
        float c0 = buffer[(iw-1+Len) & Mask];
        float c1 = buffer[iw & Mask];
        float c2 = buffer[(iw+1) & Mask];
        float c3 = buffer[(iw+2) & Mask];
        
        fw+=1.f;
        
        float ff0 = (fw-1)*(fw-2)*(fw-3)/-6*c0;
        float ff1 = fw*(fw-2)*(fw-3)/2*c1;
        float ff2 = fw*(fw-1)*(fw-3)/-2*c2;
        float ff3 = fw*(fw-1)*(fw-2)/6*c3;
        
        
        return ff0+ff1+ff2+ff3;
        
        
    }
void VarDelay::Write(float value){
        
        buffer[sample] = value;
        sample = (sample+1) & Mask;
    }
    
void VarDelay::Pluck(float mag){
        
        for(int i=0; i<Len; i++){
            buffer[i] += random.nextFloat() * mag - mag;
            
        }
        
    }
void VarDelay::Clear(){
        
        for(int i=0; i<Len; i++){
            buffer[i]=0;
            
        }
        
    }

    
    
