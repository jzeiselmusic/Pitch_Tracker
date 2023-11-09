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

float q_value = 0.00005; // variance of freq. typically very low to make convergence stable
float r_value = 0.0001; // variance of the noise. should be low in pure environments

inline float processKalmanFilter(Biquad* filter)
{
    static float p = 0;
    
    float p_update = p + q_value;
    float k = filter->getPastSampleOne() / (pow(filter->getPastSampleOne(), 2) + (r_value / p_update));
    
    float new_a = filter->getCurrentA() + k*filter->getE();
    
    p = (1.0 - (pow(filter->getPastSampleOne(), 2) / (pow(filter->getPastSampleOne(), 2) + (r_value / p_update)))) * p_update;

    if (abs(new_a) < 1.98 || abs(new_a) > 2.0) new_a = getRandom();
    
    filter->updateFrequency(new_a);
        
    return (44100.0 / (2.0*M_PI))*std::acos(new_a / 2.0) - 10.0;
}
