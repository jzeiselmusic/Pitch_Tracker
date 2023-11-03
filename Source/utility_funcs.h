/*
  ==============================================================================

    utility_funcs.h
    Created: 3 Nov 2023 5:35:26pm
    Author:  Jacob Zeisel

  ==============================================================================
*/

#pragma once
#include "enums.h"
#include <vector>

inline enums::Key find_nearest_note(float frequency)
{
    std::vector<float> middle_scale {261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 440.00, 466.16, 493.88};
    
    std::vector<float> lower_scale = middle_scale.
}
