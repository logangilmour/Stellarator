//
//  FDNReverb.cpp
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-04.
//
//
#include "FDNReverb.h"


    FDNReverb::FDNReverb(){
        for (int i = 0; i<delayCount; ++i)
        {
            delays[i].resize(len*primes[i],0);
        }
    }
  
    float FDNReverb::process(float sample){
        
        float feed[delayCount]={};
        
        float out = 0;
        
        
        for(int i=0; i<delayCount; i++){
            int W = writers[i];
            
            
            int L = primes[i]*len;
            
            W=(W+1)%L;
            
            
            int R = (W+1)%L;
            
            out += delays[i][R]/delayCount;
            
            for(int j=0; j<delayCount; j++){
                int sign = ((i%4)==(j%4)?1:-1)*(i/4==j/4?1:-1);
                
                float val = sign*delays[i][R]*0.25f;
                
                feed[j]+=val;
                
            }
            
            
            writers[i]=W;
            
        }
        
        
        for(int i=0; i<delayCount; i++){
            
            int W = writers[i];
            
            
            attenuators[i].set(0.2);
            allpass[i].set(1.f);
            
            
            delays[i][W] = allpass[i].process(softClip(sample*0.2f + attenuators[i].process(feed[i])));

        }
        
        return out;
    }

void FDNReverb::reset(){
    for(int i=0; i<delayCount; i++){
        allpass[i].reset();
        attenuators[i].reset();
        delays[i].clear();
        delays[i].resize(primes[i]*len,0);
        writers[i]=0;
    }
}


