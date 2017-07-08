//
//  Wavetable.cpp
//  Stellarator
//
//  Created by Logan Gilmour on 2017-05-28.
//
//

#include "Wavetable.h"

bool Wavetable::initialized = false;
std::array<std::array<float,Wavetable::size>,Wavetable::Levels> Wavetable::buffer = {{0}};

Wavetable::Wavetable(){
    Logger::outputDebugString("Got here");
    if(initialized){
        return;
    }
    initialized=true;

    for(int i=0; i< Levels; ++i){
        float level = (float)i/(Levels-1);
        for(int j=0;j<mips;++j){
            int len = 1<<j;
            int off = mipOffset(j);

            for(int s=0; s<len; s++){
                float wav = 0;
                float t = (float)s/len*float_Pi*2;
                
                for(int h = 1; len/h>32;++h){
                    wav+=sin(t*h)/(h+(level*level)*h*10);
                }
                
                buffer[i][off+s]=wav;
            }
        }
    }
}

int Wavetable::mipOffset(int mip){
    int offset = 0;
    for(int i=0; i<mip; ++i){
        offset = offset << 1;
        offset = offset | 1;
    }
    return offset;
}

float Wavetable::freqToMip(float freq){
    return std::log2(sampleRate/freq);
}

float Wavetable::process(float level, float freq){
    angle+=freq/sampleRate;
    float mip = freqToMip(freq);
    
    int lowMip = std::floor(mip);
    int highMip = std::ceil(mip);
    float intpart;
    mip = std::modff(mip,&intpart);
    
    int lowLevel = std::floor(level*(Levels-1));
    int highLevel = std::ceil(level*(Levels-1));
    return lerp(
        lerp(sample(lowMip,lowLevel),sample(highMip,lowLevel),mip),
        lerp(sample(lowMip,highLevel),sample(highMip,highLevel),mip),
        level
        );
    
}

float Wavetable::sample(int mip, int level){
    long len = 1<<mip;
    double index = angle*len;
    
    long lowIndex = std::floor(index);
    long highIndex = std::ceil(index);
    
    double intpart;
    index = std::modf(index,&intpart);
    
    float val1 = buffer[level][mipOffset(mip)+lowIndex%len];
    float val2 = buffer[level][mipOffset(mip)+highIndex%len];
    
    return lerp(val1,val2,index);
}



void Wavetable::reset(){
    angle=0;
    sampleRate=44100;
    
}
