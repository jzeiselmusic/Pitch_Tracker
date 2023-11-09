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
#include <random>
#include <map>

inline std::map<enums::Key, float> noteDictionary
{
    {enums::C, 261.63},
    {enums::C_s, 277.18},
    {enums::D, 293.66},
    {enums::D_s, 311.13},
    {enums::E, 329.63},
    {enums::F, 349.23},
    {enums::F_s, 369.99},
    {enums::G, 392.00},
    {enums::G_s, 415.30},
    {enums::A, 440.00},
    {enums::A_s, 466.16},
    {enums::B, 493.88}
};

inline std::tuple<int, int> find_nearest_note(float frequency, enums::Key key, enums::Scale scale)
{
    std::vector<float> middle_scale;
    switch (key) {
        case 0:
            // case ANY_KEY
            middle_scale = {261.63, 277.18, 293.66, 311.13,
                            329.63, 349.23, 369.99, 392.00,
                            415.30, 440.00, 466.16, 493.88};
            break;
        case 1:
            // case KEY C
            middle_scale = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88};
            break;
        case 2:
            // case KEY C#
            middle_scale = {277.18, 311.13, 349.23, 369.99, 415.30, 466.16, 523.25};
            break;
        case 3:
            // case KEY D
            middle_scale = {277.18, 293.66, 329.63, 369.99, 392.00, 440.00, 493.88};
            break;
        case 4:
            // case KEY D#
            middle_scale = {261.63, 293.66, 311.13, 349.23, 392.00, 415.30, 466.16};
            break;
        case 5:
            // case KEY E
            middle_scale = {277.18, 311.13, 329.63, 369.99, 415.30, 440.00, 493.88};
            break;
        case 6:
            // case KEY F
            middle_scale = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 466.16};
            break;
        case 7:
            // case KEY F#
            middle_scale = {277.18, 311.13, 349.23, 369.99, 415.30, 466.16, 493.88};
            break;
        case 8:
            // case KEY G
            middle_scale = {261.63, 293.66, 329.63, 369.99, 392.00, 440.00, 493.88};
            break;
        case 9:
            // case KEY G#
            middle_scale = {261.63, 277.18, 311.13, 349.23, 392.00, 415.30, 466.16};
            break;
        case 10:
            // case KEY A
            middle_scale = {277.18, 293.66, 329.63, 369.99, 415.30, 440.00, 493.88};
            break;
        case 11:
            // case KEY A#
            middle_scale = {261.63, 293.66, 311.13, 349.23, 392.00, 440.00, 466.16};
            break;
        case 12:
            // case KEY B
            middle_scale = {277.18, 311.13, 349.23, 369.99, 415.30, 466.16, 493.88};
            break;
        default:
            // default ANY_KEY
            middle_scale = {261.63, 277.18, 293.66, 311.13,
                            329.63, 349.23, 369.99, 392.00,
                            415.30, 440.00, 466.16, 493.88};
            break;
    }
    
    auto scale_size = middle_scale.size();
    
    std::vector<float> lower_scale;
    lower_scale.resize(scale_size);
    
    std::vector<float> lower_lower_scale;
    lower_lower_scale.resize(scale_size);
    
    std::vector<float> upper_scale;
    upper_scale.resize(scale_size);
    
    std::vector<float> upper_upper_scale;
    upper_upper_scale.resize(scale_size);
    
    for (int i = 0; i < scale_size; ++i)
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
    while (1)
    {
        if (scales[location]->at(scale_size-1) > frequency)
        {
            break;
        }
        else
        {
            location++;
        }
        if (location == scales.size())
        {
            location--;
            break;
        }
    }
        
    // now we know the closest correct note is somewhere in the list of frequencies in scales[location]
    int note_location = 0;
    while (1)
    {
        if (scales[location]->at(note_location) > frequency)
        {
            break;
        }
        else
        {
            note_location++;
        }
        if (note_location == scales[0]->size())
        {
            note_location--;
            break;
        }
    }
    
    return std::tuple<int, int>{(note_location+1), (location+1)};
}

inline float getRandom(void)
{
    // Create a random number generator engine
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribution(1.99f, 1.999f);

    // Generate a random float in the specified range
    return distribution(gen);
}

inline float getRandomInRange(float a1, float a2)
{
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribution(a1, a2);
    
    return distribution(gen);
}
