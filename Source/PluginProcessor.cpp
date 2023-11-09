/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Kalman.h"
#include "utility_funcs.h"

//==============================================================================
Pitch_Tracker_PluginAudioProcessor::Pitch_Tracker_PluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::mono(), true)
                     #endif
                       )
#endif
{
}

Pitch_Tracker_PluginAudioProcessor::~Pitch_Tracker_PluginAudioProcessor()
{
}

//==============================================================================
const juce::String Pitch_Tracker_PluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Pitch_Tracker_PluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Pitch_Tracker_PluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Pitch_Tracker_PluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Pitch_Tracker_PluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Pitch_Tracker_PluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Pitch_Tracker_PluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Pitch_Tracker_PluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Pitch_Tracker_PluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void Pitch_Tracker_PluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Pitch_Tracker_PluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_time = 0.0;
    m_deltaTime = 1.0 / sampleRate;
    monoBuffer = new float[samplesPerBlock] {0};
    RMSval.reset(getSampleRate(), 0.0005);
}

void Pitch_Tracker_PluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Pitch_Tracker_PluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void Pitch_Tracker_PluginAudioProcessor::mixWaves(float* pmonoBuffer, int numSamples) {
    Random random;

    // generate sin wave in mono
    for (int sample = 0; sample < numSamples; ++sample) {
        double triangle = my_synth.nextSample(m_time, numSamples);

        pmonoBuffer[sample] = triangle;
        m_time += m_deltaTime;
    }
}

void Pitch_Tracker_PluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // LPF and HPF setup and processing
    
    auto RMS = 0.001*buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    // if (RMS < 0.00001) RMS = 0.0;
    RMSval.setTargetValue(RMS);
    
    highPassFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 100.0);
    lowPassFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 600.0);
    
    // synth setup and processing
    
    my_synth.setFrequency(noteDictionary.at(current_note));
    my_synth.setAmplitude(-1.0*(100.0 + juce::Decibels::gainToDecibels(RMSval.getNextValue())));
    
    if (m_time >= std::numeric_limits<float>::max()) {
        m_time = 0.0;
    }

    std::fill(monoBuffer, monoBuffer + buffer.getNumSamples(), 0);
    mixWaves(monoBuffer, buffer.getNumSamples());
    
    
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        juce::dsp::AudioBlock<float> block (buffer);
        juce::dsp::ProcessContextReplacing<float> context (block);
        highPassFilter.process(context);
        lowPassFilter.process(context);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto current_sample = buffer.getSample(channel, sample);
            if (channel == 0)
            {
                biquad_filter.processInput(current_sample);
                frequency_val = (int)processKalmanFilter(&biquad_filter);
                
                channelData[sample] = current_sample + monoBuffer[sample];
            }
            if (channel == 1)
            {
            }
            // channelData[sample] = current_sample;
        }
    }
}

//==============================================================================
bool Pitch_Tracker_PluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Pitch_Tracker_PluginAudioProcessor::createEditor()
{
    return new Pitch_Tracker_PluginAudioProcessorEditor (*this);
}

//==============================================================================
void Pitch_Tracker_PluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Pitch_Tracker_PluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Pitch_Tracker_PluginAudioProcessor();
}
