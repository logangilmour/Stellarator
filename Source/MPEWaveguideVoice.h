//
//  MPEWaveguideVoice.h
//  Stellarator
//
//  Created by Logan Gilmour on 2016-12-30.
//
//

#ifndef MPEWaveguideVoice_h
#define MPEWaveguideVoice_h


class MPEWaveguideVoice : public MPESynthesiserVoice{
public:
    MPEWaveguideVoice()
    {
    }
    //==============================================================================
    void noteStarted() override
    {
        jassert (currentlyPlayingNote.isValid());
        jassert (currentlyPlayingNote.keyState == MPENote::keyDown
                 || currentlyPlayingNote.keyState == MPENote::keyDownAndSustained);
        
        // get data from the current MPENote
        level.setValue (currentlyPlayingNote.pressure.asUnsignedFloat());
        frequency.setValue (currentlyPlayingNote.getFrequencyInHertz());
        timbre.setValue (currentlyPlayingNote.timbre.asUnsignedFloat());
        float vel = currentlyPlayingNote.noteOnVelocity.asUnsignedFloat();
        vel = vel*vel;
        wave1.Pluck(vel);
        

    }
    
    void noteStopped (bool allowTailOff) override
    {
        jassert (currentlyPlayingNote.keyState == MPENote::off);
        

        clearCurrentNote();
        wave1.Clear();
        
    }
    
    void notePressureChanged() override
    {
        level.setValue (currentlyPlayingNote.pressure.asUnsignedFloat());

    }
    
    void notePitchbendChanged() override
    {
        frequency.setValue (currentlyPlayingNote.getFrequencyInHertz());
    }
    
    void noteTimbreChanged() override
    {
        timbre.setValue (currentlyPlayingNote.timbre.asUnsignedFloat());
    }
    
    void noteKeyStateChanged() override
    {
    }
    
    void setCurrentSampleRate (double newRate) override
    {
        if (currentSampleRate != newRate)
        {
            noteStopped (false);
            currentSampleRate = newRate;
            
            level.reset (currentSampleRate, smoothingLengthInSeconds);
            timbre.reset (currentSampleRate, smoothingLengthInSeconds);
            frequency.reset (currentSampleRate, smoothingLengthInSeconds);
        }
    }
    
    //==============================================================================
    void renderNextBlock (AudioBuffer<float>& outputBuffer,
                          int startSample,
                          int numSamples) override
    {

        
        
        
        
        
        float c = 0.1f;//screenY;
        
        
        for (int sample = 0; sample < numSamples; ++sample){
            float freq = frequency.getNextValue();
            float lev = 1-level.getNextValue();
            //t = oldT;
            float wlen = getSampleRate()/freq;
            
            
            
            
            
            //float ii = wave[waveR]*fw+wave[(waveR+1)%blen]*(1-fw);
            
            float ii = wave1.Read(wlen);
            
            float damper = fmin(fmax((lev-0.9f)/0.1f,0),1);
            float blaster = fmin(fmax(((1-lev)-0.5f)/0.5f,0),1);
            
            
            oldw = (ii*(0.98f-damper*0.51f)+(oldw*(0.019f+damper*0.5f)));
            
            
            
            
            float in = -oldw;
            
            float out = 0;
            int amt = allcount;

            for(int i=0; i<amt; i++){
                float con = c+(float)i/amt*0.1f;
                allnet[i] = -con*in+oldx+ con*allnet[i];
                out+=allnet[i]/amt;
            }
            
            
            oldx = in;
            
            
            float output = verb.processSample(out);
            
            float bell = SoftClip(out+output*0.05f);
            
            blaster = fmax(0,fmin(1,blaster));
            
            
            
            wave1.Write(bell*(blaster*0.05f+1.f));
            
            for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                outputBuffer.addSample (i, startSample+sample, bell*0.5f+output*0.5f);
        }
        
        
        
        
    }
    
    static float SoftClip(float v){
        float a = 0.04f;
        float b = 0.08f;
        float G = 1.01f;
        return (exp(v*(a+G))-exp(v*(b-G)))/(exp(v)+exp(-v));
        
    }
    
    
    
    float Saturate(float v){
        
        return fmin(0.99999f,fmax(-0.99999f,v));
    }
    
private:
    Random random;
    
    LinearSmoothedValue<double> level, timbre, frequency;
    
    float weird = 0;
    float oldw = 0;
    float oldx = 0;
    static const int allcount = 1;
    float allnet[allcount] = {};
    bool left = false;
    int waveW = 0;
    float oldT = 0;

    
    
    class FDN {
    public:
        
        FDN(){
            for(int i=0; i<delayCount; i++){
                delays[i] = new float[len*primes[i]];
                for(int j=0;j<len*primes[i];j++){
                    delays[i][j]=0;
                    
                }
                
            }
        }
        ~FDN(){
            for(int i=0; i<delayCount; i++){
                delete delays[i];
            }
            
        }
        float processSample(float sample){
            
            float feed[delayCount]={};
            
            float out = 0;
            
            
            for(int i=0; i<delayCount; i++){
                int W = writers[i];
                
                
                int L = primes[i]*len;
                
                W=(W+1)%L;
                
                
                int R = (W+1)%L;
                
                out += delays[i][R]/delayCount;
                String message;
                
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
        
    private:
        static const int delayCount = 16;
        static const int len = 128;
        
        bool printed[16] = {};
        
        float allpass[delayCount] = {};
        
        int primes[24] = {2,3,5,7,11,13,17,19,23,29,31,37,41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89};
        
        float *delays[delayCount];
        int writers[delayCount] = {};
        float olds[delayCount] = {};
        
    };
    
    class VarDelay{
    public:
        float Read(float delay){
            
            
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
        void Write(float value){
            
            buffer[sample] = value;
            sample = (sample+1) & Mask;
        }
        
        void Pluck(float mag){
            
            for(int i=0; i<Len; i++){
                buffer[i] += random.nextFloat() * mag - mag;
                
            }
            
        }
        void Clear(){
            
            for(int i=0; i<Len; i++){
                buffer[i]=0;
                
            }
            
        }
        
    private:
        Random random;
        
        static const int Len = 4096;
        static const int Mask = Len-1;
        float buffer[Len] = {};
        int sample = 0;
        
        
    };
    
    FDN verb;
    VarDelay wave1;
    
    const double maxLevel = 0.05f;
    const double maxLevelDb = 31.0f;
    const double smoothingLengthInSeconds = 0.01;
};

#endif /* MPEWaveguideVoice_h */
