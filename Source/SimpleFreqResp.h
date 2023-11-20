
/*
  ==============================================================================
   This file is a modified SimpleFFTDemo which is a part of the JUCE examples.
   Copyright (c) 2020 - Raw Material Software Limited
   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.
   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.
  ==============================================================================
*/

#pragma once

#include "cmp/include/include/cmp_datamodels.h"
#include "cmp/include/include/cmp_plot.h"
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>
#include <string>
#include "VerticalLine.h"

using namespace juce;

//==============================================================================
class SimpleFreqRespDemo : public AudioAppComponent, private Timer {
 public:
  SimpleFreqRespDemo()
      : audioSetupComp(deviceManager,
                       0,             // minimum input channels
                       num_channels,  // maximum input channels
                       0,             // minimum output channels
                       2,             // maximum output channels
                       false,         // ability to select midi inputs
                       false,         // ability to select midi output device
                       false,         // treat channels as stereo pairs
                       false)         // hide advanced options
        ,
        forwardFFT(fftOrder),
        window(fftSize, juce::dsp::WindowingFunction<float>::hann) {
    RuntimePermissions::request(
        RuntimePermissions::recordAudio, [this](bool granted) {
          int numInputChannels = granted ? 2 : 0;
          setAudioChannels(numInputChannels, num_channels);
        });

    addAndMakeVisible(audioSetupComp);
    addAndMakeVisible(m_plot);
    addAndMakeVisible(m_tracepoint_cb_label);

    startTimerHz(30);
    setSize(900, 400);

    m_plot.setDownsamplingType(cmp::DownsamplingType::x_downsampling);

    m_plot.setTitle("frequency information");
    m_plot.setYLabel("Power [dB]");
    m_plot.setXLabel("Frequency [Hz]");

    m_plot.yLim(-60.0f, 10.0f);
    m_plot.xLim(100.0f, 18'000.0f);

    m_plot.setLegend({"Left input", "Right input"});

    m_plot.onTraceValueChange = [this](auto*, auto previous_trace_point,
                                       auto new_trace_point) {
      this->m_tracepoint_cb_label.setText(
          "Callback from tracepoint:\nX: " +
              std::to_string(new_trace_point.getX()) +
              "\nY: " + std::to_string(new_trace_point.getY()),
                juce::NotificationType::dontSendNotification);
    };
  }

  ~SimpleFreqRespDemo() override { shutdownAudio(); }

  //==============================================================================
  void prepareToPlay(int /*samplesPerBlockExpected*/,
                     double new_sample_rate) override {
    for (auto& x : x_data) {
        std::vector<float>::iterator a = x.begin();
        std::vector<float>::iterator b = x.end();
        float c = float(new_sample_rate + 1)/ float(fftSize);
        float d = 1.0f;
        while (a != b) {
          *a++ = d;
          d += c;
        }
    }
      
    m_plot.plot(fftData, x_data);
  }

    void resized() override {
    auto rect = getLocalBounds();

    m_plot.setBounds(rect.removeFromLeft(proportionOfWidth(0.7f))
                         .removeFromTop(proportionOfHeight(0.85f)));

    audioSetupComp.setBounds(rect.removeFromRight(proportionOfWidth(0.3f))
                                 .removeFromTop(proportionOfHeight(0.85f)));

    rect = getLocalBounds();
    m_tracepoint_cb_label.setBounds(
        rect.removeFromBottom(proportionOfHeight(0.15f)));
    }

    void releaseResources() override {
    // (nothing to do here)
    }

    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override {
    if (bufferToFill.buffer->getNumChannels() > 0) {
      const auto* channelData =
          bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

      for (auto ch_idx = 0u; ch_idx != num_channels; ++ch_idx) {
        for (auto i = 0; i < bufferToFill.numSamples; ++i)
          pushNextSampleIntoFifo(channelData[i], ch_idx);
      }

      bufferToFill.clearActiveBufferRegion();
    }
    }

    //==============================================================================
    
    void paint(Graphics& g) override {
        g.setColour(juce::Colours::grey);

        g.drawRoundedRectangle(m_plot.getBounds().toFloat(), 5.0f, 5.0f);
        g.drawRoundedRectangle(m_tracepoint_cb_label.getBounds().toFloat(), 5.0f,
                           5.0f);
        g.drawRoundedRectangle(audioSetupComp.getBounds().toFloat(), 5.0f, 5.0f);
    }

    void timerCallback() override {
        if (nextFFTBlockReady) {
            for (size_t i = 0; i < num_channels; i++) {
                calcNextFrequencyResponse(i);
            }

            m_plot.realTimePlot(fftData);
            // m_plot.toFront(false);
            nextFFTBlockReady = false;
            
            m_plot.setXTicks({current_calculated_frequency});
            m_plot.repaint();
        }
    }

    void pushNextSampleIntoFifo(const float sample,
                              const std::size_t ch_idx) noexcept {
        if (fifoIndex[ch_idx] == fftSize) {
            if (!nextFFTBlockReady) {
                std::copy(fifo[ch_idx].begin(), fifo[ch_idx].end(),
                          fftData[ch_idx].begin());
                nextFFTBlockReady = true;
            }

            fifoIndex[ch_idx] = 0;
        }

        fifo[ch_idx][fifoIndex[ch_idx]++] = sample;
    }

    void calcNextFrequencyResponse(const std::size_t ch_idx) {
        window.multiplyWithWindowingTable(fftData[ch_idx].data(), fftSize);  // [1]

        forwardFFT.performFrequencyOnlyForwardTransform(fftData[ch_idx].data());

        constexpr auto scale = 1.0f / float(fftSize);

        constexpr auto smoothing_factor = 0.5f;

        auto it_smooth = fftDataSmooth[ch_idx].begin();
        for (auto& s : fftData[ch_idx]) {
            s = s * scale;

            *it_smooth++ = s = (*it_smooth + s) * smoothing_factor;

            if (s < 1e-7f) {
                s = -70;
                continue;
            }

            constexpr auto smoothing_compensation = 10.0f;

            s = 10.0f * log10f(s) + smoothing_compensation;
        }
    }

    enum { fftOrder = 11, fftSize = 1 << fftOrder };

    void setCalculatedFrequency(float new_hz) {
        current_calculated_frequency = new_hz;
    }
    
 private:
    static constexpr int num_channels{1};

    juce::AudioDeviceSelectorComponent audioSetupComp;

    juce::dsp::WindowingFunction<float> window;

    dsp::FFT forwardFFT;

    std::vector<std::vector<float>> fifo =
      decltype(fifo)(num_channels, std::vector<float>(fftSize));

    decltype(fifo) x_data =
      decltype(fifo)(num_channels, std::vector<float>(2 * fftSize));

    decltype(fifo) fftData =
      decltype(fifo)(num_channels, std::vector<float>(2 * fftSize));

    decltype(fifo) fftDataSmooth =
      decltype(fifo)(num_channels, std::vector<float>(2 * fftSize));

    std::vector<int> fifoIndex = std::vector<int>(num_channels);

    bool nextFFTBlockReady = false;

    cmp::SemiLogX m_plot;

    juce::Label m_tracepoint_cb_label;
    
    float current_calculated_frequency = 0.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleFreqRespDemo)
};
