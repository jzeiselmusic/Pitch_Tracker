/*
  ==============================================================================

    Synth.h
    Created: 5 Nov 2023 12:26:32pm
    Author:  Jacob Zeisel

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "IWave.h"

class JZSynth : public IWave
{
public:
    JZSynth() {};
    ~JZSynth() {};
    
    double sample(double t, int len) override {
        double res = 0.0;
        double fullPeriodTime = 1.0 / frequency;
        double localTime = fmod(t, fullPeriodTime);

        double value = localTime / fullPeriodTime;

        if (value < 0.25)
        {
            res = value * 4;
        }
        else if (value < 0.75)
        {
            res = 2.0 - (value * 4.0);
        }
        else
        {
            res = value * 4 - 4.0;
        }

        return level * res;
    };
};
