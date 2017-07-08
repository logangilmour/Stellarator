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

    repaint();
}

void StellaratorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

