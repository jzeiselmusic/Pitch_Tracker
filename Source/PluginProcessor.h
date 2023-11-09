/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Biquad.h"
#include "enums.h"
#include "Synth.h"

//==============================================================================
/**
*/
class Pitch_Tracker_PluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Pitch_Tracker_PluginAudioProcessor();
    ~Pitch_Tracker_PluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    float frequency_val = 0.0;
    
    Biquad* getFilter(void) { return &(this->biquad_filter);};
    void mixWaves(float* pmonoBuffer, int numSamples);
    
    enums::Key current_key = enums::C;
    enums::Scale current_scale = enums::Major;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pitch_Tracker_PluginAudioProcessor)
    
    Biquad biquad_filter = Biquad();
    
    juce::dsp::IIR::Filter<float> highPassFilter;
    juce::dsp::IIR::Filter<float> lowPassFilter;
    
    JZSynth my_synth = JZSynth();
    
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> RMSval;
    
    double m_time;
    double m_deltaTime;
    float *monoBuffer;
    
};
