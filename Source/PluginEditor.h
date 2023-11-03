/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Frequency_Display.h"

//==============================================================================
/**
*/
class Pitch_Tracker_PluginAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer, public juce::Slider::Listener
{
public:
    Pitch_Tracker_PluginAudioProcessorEditor (Pitch_Tracker_PluginAudioProcessor&);
    ~Pitch_Tracker_PluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void sliderValueChanged(juce::Slider*) override;
    void keyBoxChanged(void);
    void scaleBoxChanged(void);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Pitch_Tracker_PluginAudioProcessor& audioProcessor;
    
    Gui::Frequency_Display freq_rect;
    
    juce::Slider frequency_variance_slider;
    juce::Label frequency_variance_label;
    
    juce::Slider noise_variance_slider;
    juce::Label noise_variance_label;
    
    juce::Slider q_factor_slider;
    juce::Label q_factor_label;
    
    juce::ComboBox choose_key;
    juce::ComboBox choose_scale;
    
    int Window_X = 500;
    int Window_Y = 400;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pitch_Tracker_PluginAudioProcessorEditor)
};
