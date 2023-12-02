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
#include "enums.h"

class Kalman {
public:
    Kalman(Biquad* b_filter, enums::Key key, enums::Scale scale) {
        filter = b_filter;
        current_key = key;
        current_scale = scale;
    };
    ~Kalman() {};
    float process(float sampleRate, float offSet) {
        float p_update = p + q_value;
        float k = filter->getPastSampleOne() /
                            (pow(filter->getPastSampleOne(), 2) +
                            (r_value / p_update));
        
        float new_a = filter->getCurrentA() + k * filter->getE();
        
        p = (1.0 - (pow(filter->getPastSampleOne(), 2) /
                            (pow(filter->getPastSampleOne(), 2) +
                            (r_value / p_update)))) * p_update;
        
        if (abs(new_a) < 1.95 || abs(new_a) >= 2.0) new_a = 1.99;
        filter->updateFrequency(new_a);
        return convert_to_hz(new_a, sampleRate) + offSet;
    }
    
    void reset(Biquad* filter, enums::Key key, enums::Scale scale) {
        this->current_key = key;
        this->current_scale = scale;
        this->p = 0.0;
        filter->reset();
    }
    
    void set_scale_and_key(enums::Key key, enums::Scale scale) {this->current_key = key; this->current_scale = scale;};
    void set_q_value(float new_value) {this->q_value = new_value;};
    void set_r_value(float new_value) {this->r_value = new_value;};
private:
    
    Biquad* filter;
    
    // set default values. can be changed by user
    float q_value = 0.00005;
    float r_value = 0.0001;
    float p = 0.0;
    
    enums::Key current_key = enums::C;
    enums::Scale current_scale = enums::Major;
};

