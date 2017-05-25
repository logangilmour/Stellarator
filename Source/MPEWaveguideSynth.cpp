//
//  MPEWaveguideSynth.cpp
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-07.
//
//

#include "MPEWaveguideSynth.h"
#include "MPEWaveguideVoice.h"
#include "DspUtil.h"

void MPEWaveguideSynth::renderNextSubBlock (AudioBuffer<float>& buffer, int startSample, int numSamples)
{
    for(int sample = 0; sample<numSamples; ++sample)
    {
        float total = 0;
        for (int i = voices.size(); --i >= 0;)
        {
            MPEWaveguideVoice* voice = static_cast<MPEWaveguideVoice*>(voices.getUnchecked (i));
            
            if (voice->isActive()){
                total+=voice->process(reverbFeedback);
            }
            
        }
        total = softClip(total*0.3);
        reverbFeedback=reverb.process(total);
        buffer.addSample(0, sample+startSample,reverbFeedback*5);
    }


    for(int i=1; i<buffer.getNumChannels(); ++i){
        for(int sample = 0; sample<numSamples; ++sample){
        buffer.addSample(i,startSample+sample , buffer.getSample(0,sample+startSample));
        }
    }

}

struct MPEVoiceAgeSorter
{
    static int compareElements (MPEWaveguideVoice* v1, MPEWaveguideVoice* v2) noexcept
    {
        return v1->compareTo(v2);
    }
};

MPESynthesiserVoice* MPEWaveguideSynth::findFreeVoice (MPENote noteToFindVoiceFor, bool stealIfNoneAvailable) const
{
    
    for (int i = 0; i < voices.size(); ++i)
    {
        MPESynthesiserVoice* const voice = voices.getUnchecked (i);
        
        if (! voice->isActive() || voice->isCurrentlyPlayingNote(noteToFindVoiceFor))
            return voice;
    }
    
    if (stealIfNoneAvailable)
        return findVoiceToSteal (noteToFindVoiceFor);
    
    return nullptr;
}

MPESynthesiserVoice* MPEWaveguideSynth::findVoiceToSteal (MPENote noteToStealVoiceFor) const
{
    // This voice-stealing algorithm applies the following heuristics:
    // - Re-use the oldest notes first
    // - Protect the lowest & topmost notes, even if sustained, but not if they've been released.
    
    
    // apparently you are trying to render audio without having any voices...
    jassert (voices.size() > 0);
    
    // These are the voices we want to protect (ie: only steal if unavoidable)
    MPESynthesiserVoice* low = nullptr; // Lowest sounding note, might be sustained, but NOT in release phase
    MPESynthesiserVoice* top = nullptr; // Highest sounding note, might be sustained, but NOT in release phase
    
    // this is a list of voices we can steal, sorted by how long they've been running
    Array<MPEWaveguideVoice*> usableVoices;
    usableVoices.ensureStorageAllocated (voices.size());
    
    for (int i = 0; i < voices.size(); ++i)
    {
        MPEWaveguideVoice* const voice = static_cast<MPEWaveguideVoice*>(voices.getUnchecked (i));
        jassert (voice->isActive()); // We wouldn't be here otherwise
        
        MPEVoiceAgeSorter sorter;
        usableVoices.addSorted (sorter, voice);
        
        if (! voice->isPlayingButReleased()) // Don't protect released notes
        {
            const int noteNumber = voice->getCurrentlyPlayingNote().initialNote;
            
            if (low == nullptr || noteNumber < low->getCurrentlyPlayingNote().initialNote)
                low = voice;
            
            if (top == nullptr || noteNumber > top->getCurrentlyPlayingNote().initialNote)
                top = voice;
        }
    }
    
    // Eliminate pathological cases (ie: only 1 note playing): we always give precedence to the lowest note(s)
    if (top == low)
        top = nullptr;
    
    const int numUsableVoices = usableVoices.size();
    
    // If we want to re-use the voice to trigger a new note,
    // then The oldest note that's playing the same note number is ideal.
    if (noteToStealVoiceFor.isValid())
    {
        for (int i = 0; i < numUsableVoices; ++i)
        {
            MPESynthesiserVoice* const voice = usableVoices.getUnchecked (i);
            float pitch = voice->getCurrentlyPlayingNote().pitchbend.asSignedFloat();
            if (voice->getCurrentlyPlayingNote().initialNote == noteToStealVoiceFor.initialNote && pitch<0.01 && pitch > -0.01){
                return voice;
            }
        }
    }
    
    // Oldest voice that has been released (no finger on it and not held by sustain pedal)
    for (int i = 0; i < numUsableVoices; ++i)
    {
        
        MPESynthesiserVoice* const voice = usableVoices.getUnchecked (i);
        
        
        if (voice != low && voice != top && voice->isPlayingButReleased())
            return voice;
    }
    
    // Oldest voice that doesn't have a finger on it:
    for (int i = 0; i < numUsableVoices; ++i)
    {
        MPESynthesiserVoice* const voice = usableVoices.getUnchecked (i);
        
        if (voice != low && voice != top
            && voice->getCurrentlyPlayingNote().keyState != MPENote::keyDown
            && voice->getCurrentlyPlayingNote().keyState != MPENote::keyDownAndSustained)
            return voice;
    }
    
    // Oldest voice that isn't protected
    for (int i = 0; i < numUsableVoices; ++i)
    {
        MPESynthesiserVoice* const voice = usableVoices.getUnchecked (i);
        
        if (voice != low && voice != top)
            return voice;
    }
    
    // We've only got "protected" voices now: lowest note takes priority
    jassert (low != nullptr);
    
    // Duophonic synth: give priority to the bass note:
    if (top != nullptr)
        return top;
    
    return low;
}
