/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "VoicePanel.h"


//==============================================================================
/**
*/
class StellaratorAudioProcessorEditor  : public AudioProcessorEditor, private Timer
{
public:
    StellaratorAudioProcessorEditor (StellaratorAudioProcessor&, AudioProcessorValueTreeState&);
    ~StellaratorAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StellaratorAudioProcessor& processor;
    AudioProcessorValueTreeState& parameters;
    void timerCallback() override;
    
    VoicePanel voicePanel;
    
    Slider outputSlider;
    ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachment;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StellaratorAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
