//
//  FDNReverb.cpp
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-04.
//
//
#include "FDNReverb.h"


    FDNReverb::FDNReverb(){
        for(int i=0; i<delayCount; i++){
            delays[i] = new float[len*primes[i]];
            for(int j=0;j<len*primes[i];j++){
                delays[i][j]=0;
                
            }
            
        }
    }
  
    float FDNReverb::processSample(float sample){
        
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
            
            
            
            olds[i] = 0.8*olds[i]+0.2*feed[i];
            float c = 1.f;
            
            
            float input = SoftClip(sample*0.2f + olds[i]);//olds[i];
            
            
            
            float output1 = allpass[i]+c*(input-c*allpass[i]);
            
            allpass[i] = input-c*allpass[i];
            
            
            
            
            delays[i][W] = output1;
            
            
            
        }
        
        return out;
    }

    

