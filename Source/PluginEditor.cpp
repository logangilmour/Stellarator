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
StellaratorAudioProcessorEditor::StellaratorAudioProcessorEditor (StellaratorAudioProcessor& p, AudioProcessorValueTreeState& a)
    : AudioProcessorEditor (&p), processor (p), parameters(a)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //addAndMakeVisible(voicePanel);
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
    
    addAndMakeVisible (outputSlider);
    outputSlider.setBounds(0, 0, 100, 100);
    outputAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment (parameters, "output", outputSlider);
    

}

void StellaratorAudioProcessorEditor::timerCallback()
{

    repaint();
}

void StellaratorAudioProcessorEditor::resized()
{
    //voicePanel.setBounds(getBounds().reduced(10, 10));
}

