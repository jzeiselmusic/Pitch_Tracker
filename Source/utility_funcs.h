/*
  ==============================================================================

    utility_funcs.h
    Created: 3 Nov 2023 5:35:26pm
    Author:  Jacob Zeisel

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
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

inline std::map<float, enums::Key> noteDictionaryRev
{
    {261.63, enums::C},
    {277.18, enums::C_s},
    {293.66, enums::D},
    {311.13, enums::D_s},
    {329.63, enums::E},
    {349.23, enums::F},
    {369.99, enums::F_s},
    {392.00, enums::G},
    {415.30, enums::G_s},
    {440.00, enums::A},
    {466.16, enums::A_s},
    {493.88, enums::B}
};

inline std::tuple<int, int> find_nearest_note(float frequency, enums::Key key, enums::Scale scale)
{
    std::vector<float> middle_scale;
    switch (key) {
        case enums::Key::ANY_KEY :
            middle_scale = {261.63, 277.18, 293.66, 311.13,
                            329.63, 349.23, 369.99, 392.00,
                            415.30, 440.00, 466.16, 493.88};
            break;
        case enums::Key::C :
            middle_scale = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88};
            break;
        case enums::Key::C_s :
            middle_scale = {261.63, 277.18, 311.13, 349.23, 369.99, 415.30, 466.16};
            break;
        case enums::Key::D :
            middle_scale = {277.18, 293.66, 329.63, 369.99, 392.00, 440.00, 493.88};
            break;
        case enums::Key::D_s :
            middle_scale = {261.63, 293.66, 311.13, 349.23, 392.00, 415.30, 466.16};
            break;
        case enums::Key::E :
            middle_scale = {277.18, 311.13, 329.63, 369.99, 415.30, 440.00, 493.88};
            break;
        case enums::Key::F :
            middle_scale = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 466.16};
            break;
        case enums::Key::F_s :
            middle_scale = {277.18, 311.13, 349.23, 369.99, 415.30, 466.16, 493.88};
            break;
        case enums::Key::G :
            middle_scale = {261.63, 293.66, 329.63, 369.99, 392.00, 440.00, 493.88};
            break;
        case enums::Key::G_s :
            middle_scale = {261.63, 277.18, 311.13, 349.23, 392.00, 415.30, 466.16};
            break;
        case enums::Key::A :
            middle_scale = {277.18, 293.66, 329.63, 369.99, 415.30, 440.00, 493.88};
            break;
        case enums::Key::A_s :
            middle_scale = {261.63, 293.66, 311.13, 349.23, 392.00, 440.00, 466.16};
            break;
        case enums::Key::B :
            middle_scale = {277.18, 311.13, 349.23, 369.99, 415.30, 466.16, 493.88};
            break;
        default:
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
    
    enums::Key final_key = noteDictionaryRev.at(middle_scale[note_location]);
    
    return std::tuple<int, int>{(int)final_key, (location+1)};
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

inline float convert_to_hz(float a, float sampleRate) {
    return (sampleRate / (2.0*M_PI))*std::acos(a / 2.0);
}

inline float convert_from_hz(float hz, float sampleRate) {
    return 2.0*std::cos((hz * 2.0 * M_PI / sampleRate));
}
