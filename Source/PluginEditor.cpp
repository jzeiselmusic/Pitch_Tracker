/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "enums.h"
#include <thread>

extern float r_value;
extern float q_value;

//==============================================================================
Pitch_Tracker_PluginAudioProcessorEditor::Pitch_Tracker_PluginAudioProcessorEditor (Pitch_Tracker_PluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (Window_X, Window_Y);
    
    addAndMakeVisible(freq_rect);
    
    addAndMakeVisible(frequency_variance_slider);
    frequency_variance_slider.setRange(0.00000001, 10.0);
    frequency_variance_slider.setSkewFactor(0.1);
    frequency_variance_slider.addListener(this);
    addAndMakeVisible(frequency_variance_label);
    frequency_variance_label.setText("freq. variance", juce::dontSendNotification);
    frequency_variance_label.attachToComponent(&frequency_variance_slider, true);
    frequency_variance_slider.setValue(0.0000001);
    
    addAndMakeVisible(noise_variance_slider);
    noise_variance_slider.setRange(0.00000001, 10.0);
    noise_variance_slider.addListener(this);
    noise_variance_slider.setSkewFactor(0.1);
    addAndMakeVisible(noise_variance_label);
    noise_variance_label.setText("noise variance", juce::dontSendNotification);
    noise_variance_label.attachToComponent(&noise_variance_slider, true);
    noise_variance_slider.setValue(0.003);
    
    addAndMakeVisible(q_factor_slider);
    q_factor_slider.setRange(0.01, 1.0);
    q_factor_slider.addListener(this);
    addAndMakeVisible(q_factor_label);
    q_factor_label.setText("Q factor", juce::dontSendNotification);
    q_factor_label.attachToComponent(&q_factor_slider, true);
    q_factor_slider.setValue(0.1);
    
    addAndMakeVisible(choose_key);
    choose_key.setTitle("Key");
    choose_key.addItemList({"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"}, 1);
    choose_key.onChange = [this] { keyBoxChanged(); };
    
    addAndMakeVisible(choose_scale);
    choose_scale.setTitle("Scale");
    choose_scale.addItemList({"Major","Minor"}, 1);
    choose_scale.onChange = [this] { scaleBoxChanged(); };
    
    startTimerHz(5);
}

Pitch_Tracker_PluginAudioProcessorEditor::~Pitch_Tracker_PluginAudioProcessorEditor()
{
}

//==============================================================================
void Pitch_Tracker_PluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void nearest_note_thread(float frequency, Gui::Frequency_Display* freq_rect, Pitch_Tracker_PluginAudioProcessorEditor* p)
{
    std::tuple<int, int> note = find_nearest_note(frequency, p->getCurrentKey(), p->getCurrentScale());
    int actual_note = get<0>(note);
    int actual_octave = get<1>(note);
    
    freq_rect->setNoteAndOctave(actual_note, actual_octave);
    p->updateCurrentNote((enums::Key)actual_note);
}

void Pitch_Tracker_PluginAudioProcessorEditor::updateCurrentNote(enums::Key currentNote)
{
    audioProcessor.current_note = currentNote;
}

enums::Key Pitch_Tracker_PluginAudioProcessorEditor::getCurrentKey(void)
{
    return audioProcessor.current_key;
}

enums::Scale Pitch_Tracker_PluginAudioProcessorEditor::getCurrentScale(void)
{
    return audioProcessor.current_scale;
}

void Pitch_Tracker_PluginAudioProcessorEditor::timerCallback()
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    freq_rect.setVal(audioProcessor.frequency_val);
    
    std::thread nearest_note_task(nearest_note_thread, audioProcessor.frequency_val, &freq_rect, this);
    freq_rect.repaint();
    nearest_note_task.detach();
}

void Pitch_Tracker_PluginAudioProcessorEditor::sliderValueChanged(juce::Slider * slider)
{
    if (slider == &frequency_variance_slider)
    {
        q_value = frequency_variance_slider.getValue();
    }
    else if (slider == &noise_variance_slider)
    {
        r_value = noise_variance_slider.getValue();
    }
    else if (slider == &q_factor_slider)
    {
        audioProcessor.getFilter()->setRho(q_factor_slider.getValue());
    }
}

void Pitch_Tracker_PluginAudioProcessorEditor::keyBoxChanged()
{
    audioProcessor.current_key = (enums::Key)choose_key.getSelectedId();
}

void Pitch_Tracker_PluginAudioProcessorEditor::scaleBoxChanged()
{
    audioProcessor.current_scale = (enums::Scale)choose_scale.getSelectedId();
}

void Pitch_Tracker_PluginAudioProcessorEditor::resized()
{
    auto sliderLeftHand = this->getLocalBounds().getWidth()/3.0;
    auto sliderTop = this->getLocalBounds().getHeight()/10.0;
    auto sliderHeight = 20.0;
    auto sliderWidth = this->getLocalBounds().getWidth()/2.0;
    frequency_variance_slider.setBounds(sliderLeftHand, sliderTop, sliderWidth, sliderHeight);
    noise_variance_slider.setBounds(sliderLeftHand, sliderTop + 2*sliderHeight, sliderWidth, sliderHeight);
    q_factor_slider.setBounds(sliderLeftHand, sliderTop + 4*sliderHeight, sliderWidth, sliderHeight);
    freq_rect.setBounds(this->getLocalBounds().getX(), this->getLocalBounds().getY(), Window_X, Window_Y);
    
    choose_key.setBounds(this->getLocalBounds().getWidth()/4, 275, 100, 65);
    choose_scale.setBounds(2*(this->getLocalBounds().getWidth()/4) + 30, 275, 100, 65);
}
