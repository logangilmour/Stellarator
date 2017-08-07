/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MPEWaveguideVoice.h"


//==============================================================================
StellaratorAudioProcessor::StellaratorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
 parameters(*this, nullptr)
{
    for(int i=0; i<15; ++i)
    {
        synth.addVoice(new MPEWaveguideVoice);
    }
    synth.enableLegacyMode (48);
    synth.setVoiceStealingEnabled (true);
    
    parameters.createAndAddParameter("output", "Output", String(), NormalisableRange<float>(0.0f,5.0f), 0.5f, nullptr, nullptr);
    
    parameters.state = ValueTree(Identifier("Stellerator"));
}

StellaratorAudioProcessor::~StellaratorAudioProcessor()
{
}

//==============================================================================
const String StellaratorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StellaratorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StellaratorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double StellaratorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StellaratorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StellaratorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StellaratorAudioProcessor::setCurrentProgram (int index)
{
}

const String StellaratorAudioProcessor::getProgramName (int index)
{
    return String();
}

void StellaratorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void StellaratorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void StellaratorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StellaratorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void StellaratorAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    
    
    
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    synth.setParams(&parameters);
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
  
    
}

//==============================================================================
bool StellaratorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* StellaratorAudioProcessor::createEditor()
{
    return new StellaratorAudioProcessorEditor (*this,parameters);
}

//==============================================================================
void StellaratorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    ScopedPointer<XmlElement> xml (parameters.state.createXml());
    copyXmlToBinary (*xml, destData);
}

void StellaratorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.state = ValueTree::fromXml (*xmlState);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StellaratorAudioProcessor();
}
