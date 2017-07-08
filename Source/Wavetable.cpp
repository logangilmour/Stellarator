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
        float lev = (float)i/(Levels-1);
        bool gotit = false;
        for(int j=0;j<mips;++j){
            int mip = j;
            int len = mipSize(mip);
            int off = mipOffset(mip);

            for(int s=0; s<len; s++){
                float wav = 0;
                float angle = (float)s/len*float_Pi*2;
                
                for(int h = 1; len/h>1;++h){
                    int harmonic = h-1;
                    //wav+=sin(t*h)/(h+(level*level)*h*10)*(1-level);
                    //wav+=sin(angle*(h+1))/(h+1+(1-level*level)*h*10)*level;
                    //wav+=sin(angle*h)/(h+(1-lev*lev)*(h-1)*10);
                    wav+=sin(angle*(harmonic+1))/(harmonic+1+(1-lev*lev)*harmonic*10);


                }
                
                buffer[i][off+s]=wav;
                if(off+s==size-1){
                    gotit=true;
                }
                if(off+s>=size){
                    Logger::outputDebugString("Oh fuck");
                }
                
            }
        }
        if(gotit){
            Logger::outputDebugString("GOOD");
        }
    }
}

int Wavetable::mipOffset(int mip){
    return (1<<mip)-1;
}

int Wavetable::mipSize(int mip){
    return 1<<mip;
}


float Wavetable::freqToMip(float freq){
    return std::log2(44100/freq)-1;
}

float Wavetable::process(float level, float freq){
    angle+=freq/sampleRate;
    float mip = freqToMip(freq);
    
    int lowMip = std::floor(mip);
    int highMip = std::ceil(mip);
    float mipblend = 1-(mip-lowMip);
    
    float levelIndex = level*(Levels-1);
    
    int lowLevel = std::floor(levelIndex);
    int highLevel = std::ceil(levelIndex);
    
    float levelBlend = 1-(levelIndex - lowLevel);
    
    return lerp(
        lerp(sample(lowMip,lowLevel),sample(highMip,lowLevel),mipblend),
        lerp(sample(lowMip,highLevel),sample(highMip,highLevel),mipblend),
        levelBlend
        );
    
}

float Wavetable::sample(int mip, int level){
    long len = mipSize(mip);
    
    double index = angle*len;
    
    long lowIndex = std::floor(index);
    long highIndex = std::ceil(index);
    
    double intpart;
    float sampleBlend = 1-std::modf(index,&intpart);
    
    float val1 = buffer[level][mipOffset(mip)+lowIndex%len];
    float val2 = buffer[level][mipOffset(mip)+highIndex%len];
    
    return lerp(val1,val2,sampleBlend);
}



void Wavetable::reset(){
    angle=0;
    sampleRate=44100;
    
}
