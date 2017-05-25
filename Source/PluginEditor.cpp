/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MPEWaveguideVoice.h"
#include <string>


//==============================================================================
StellaratorAudioProcessorEditor::StellaratorAudioProcessorEditor (StellaratorAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    startTimerHz(30);
    
}

StellaratorAudioProcessorEditor::~StellaratorAudioProcessorEditor()
{
}

//==============================================================================
void StellaratorAudioProcessorEditor::paint (Graphics& g)
{
    std::string message ("Freq: ");
    
    g.fillAll (Colours::white);

}

void StellaratorAudioProcessorEditor::timerCallback()
{
    int voices = processor.synth.getNumVoices();
    for(int i=0; i<voices; i++){
        auto voice = static_cast<MPEWaveguideVoice*>(processor.synth.getVoice(i));
        Logger::outputDebugString ("Vol "+std::to_string(i)+": "+std::to_string(voice->vol));
    }

    repaint();
}

void StellaratorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

