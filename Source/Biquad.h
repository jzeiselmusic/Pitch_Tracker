/*
  ==============================================================================

    Biquad.h
    Created: 28 Oct 2023 8:58:49pm
    Author:  Jacob Zeisel

  ==============================================================================
*/

#pragma once
#include <tuple>

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
    
private:
    
    float past_a = 0;
    float current_a = 0;
    
    float past_sample_1 = 0;
    float past_sample_2 = 0;
    
    /* rho is a Q hyperparameter. narrower as p -> 1 */
    float rho = 0.5;
    
    float s;
    float e;
    
};
