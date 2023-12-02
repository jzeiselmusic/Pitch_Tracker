/*
  ==============================================================================

    Biquad.h
    Created: 28 Oct 2023 8:58:49pm
    Author:  Jacob Zeisel

  ==============================================================================
*/

#pragma once
#include <tuple>
#include "utility_funcs.h"

class Biquad {
    
public:
    Biquad();
    ~Biquad();
    
    void updateFrequency(float new_a);
    void processInput(float input_y); // output s[n] and e[n]
    
    float getPastSampleOne(void);
    float getPastSampleTwo(void);
    float getS(void);
    float getE(void);
    float getCurrentA(void);
    
    void setRho(float new_val);
    
    void reset(void) {
        past_a = getRandom();
        current_a = getRandom();
        past_sample_1 = 0.0;
        past_sample_2 = 0.0;
    }
    
private:
    
    float past_a = getRandom();
    float current_a = getRandom();
    
    float past_sample_1 = 0;
    float past_sample_2 = 0;
    
    /* rho is a Q hyperparameter. narrower as p -> 1 */
    float rho = 0.5;
    
    float s;
    float e;
    
};
