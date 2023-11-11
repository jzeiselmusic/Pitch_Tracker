/*
  ==============================================================================

    Kalman.h
    Created: 28 Oct 2023 9:18:05pm
    Author:  Jacob Zeisel

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Biquad.h"
#include "utility_funcs.h"

class Kalman {
public:
    Kalman(Biquad* filter) {this->filter = filter;};
    ~Kalman() {};
    float process(float sampleRate, float offSet) {
        float p_update = this->p + this->q_value;
        float k = filter->getPastSampleOne() / (pow(filter->getPastSampleOne(), 2) + (this->r_value / p_update));
        float new_a = filter->getCurrentA() + k * filter->getE();
        this->p = (1.0 - (pow(filter->getPastSampleOne(), 2) / (pow(filter->getPastSampleOne(), 2) + (this->r_value / p_update)))) * p_update;
        if (abs(new_a) < 1.98 || abs(new_a) > 2.0) new_a = getRandom();
        filter->updateFrequency(new_a);
        return convert_to_hz(new_a, sampleRate) + offSet;
    }
    
    void reset(Biquad* filter) {
        this->p = 0.0;
        filter->reset();
    }
    
    float convert_to_hz(float a, float sampleRate) {
        return (sampleRate / (2.0*M_PI))*std::acos(a / 2.0);
    }
    
    void set_q_value(float new_value) {this->q_value = new_value;};
    void set_r_value(float new_value) {this->r_value = new_value;};
    
private:
    Biquad* filter;
    float q_value = 0.00005;
    float r_value = 0.0001;
    float p = 0.0;
};
