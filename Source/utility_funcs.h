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
#include <tuple>

inline std::tuple<enums::Key, enums::Octave> find_nearest_note(float frequency)
{
    std::vector<float> middle_scale {261.63, 277.18, 293.66, 311.13,
                                     329.63, 349.23, 369.99, 392.00,
                                     415.30, 440.00, 466.16, 493.88};
    
    std::vector<float> lower_scale;
    lower_scale.resize(middle_scale.size());
    
    std::vector<float> lower_lower_scale;
    lower_lower_scale.resize(middle_scale.size());
    
    std::vector<float> upper_scale;
    upper_scale.resize(middle_scale.size());
    
    std::vector<float> upper_upper_scale;
    upper_upper_scale.resize(middle_scale.size());
    
    for (int i = 0; i < middle_scale.size(); ++i)
    {
        lower_scale[i] = middle_scale[i] / 2.0;
        lower_lower_scale[i] = middle_scale[i] / 4.0;
        upper_scale[i] = middle_scale[i] * 2.0;
        upper_upper_scale[i] = middle_scale[i] * 4.0;
    }
    
    std::vector<std::vector<float>*> scales = {&lower_lower_scale, &lower_scale,
                                               &middle_scale, &upper_scale,
                                               &upper_upper_scale};
    
    int location = 0;
    while (location < scales.size())
    {
        if (scales[location]->at(0) > frequency)
        {
            break;
        }
        else
        {
            location++;
        }
        if (location == scales.size() - 1)
        {
            location--;
        }
    }
    
    //std::cout << "location: " << location << "\n";
    
    // now we know the closest correct note is somewhere in the list of frequencies in scales[location]
    int note_location = 0;
    //std::cout << "size: " << scales[location]->size() << "\n";
    while (note_location < scales[0]->size())
    {
        //std::cout << "note location: " << note_location << "\n";
        if (scales[location]->at(note_location) > frequency)
        {
            break;
        }
        else
        {
            note_location++;
        }
        if (note_location == scales[0]->size() - 1)
        {
            note_location--;
        }
    }
    
    return std::tuple<enums::Key, enums::Octave>{(enums::Key)(note_location+1), (enums::Octave)(location+1)};
}
