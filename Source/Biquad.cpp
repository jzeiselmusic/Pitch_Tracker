/*
  ==============================================================================

    Biquad.cpp
    Created: 28 Oct 2023 8:58:43pm
    Author:  Jacob Zeisel

  ==============================================================================
*/

#include "Biquad.h"

Biquad::Biquad(void)
{
    s = 0;
    e = 0;
}

Biquad::~Biquad(void)
{
}

float Biquad::getPastSampleOne()
{
    return past_sample_1;
}

float Biquad::getPastSampleTwo()
{
    return past_sample_2;
}

float Biquad::getS()
{
    return s;
}

float Biquad::getE()
{
    return e;
}

float Biquad::getCurrentA()
{
    return current_a;
}

void Biquad::setRho(float new_val)
{
    rho = new_val;
}

void Biquad::updateFrequency(float new_a)
{
    this->past_a = this->current_a;
    this->current_a = new_a;
}

void Biquad::processInput(float input_y)
{
    s = input_y + rho*current_a*past_sample_1 - pow(rho, 2)*past_sample_2;
    e = s - current_a*past_sample_1 + past_sample_2;
    past_sample_2 = past_sample_1;
    past_sample_1 = s;
}

