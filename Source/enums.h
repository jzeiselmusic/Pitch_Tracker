/*
  ==============================================================================

    enums.h
    Created: 3 Nov 2023 1:13:25pm
    Author:  Jacob Zeisel

  ==============================================================================
*/

#pragma once

namespace enums {

typedef enum Key {
    ANY_KEY = 0,
    C = 1,
    C_s = 2,
    D = 3,
    D_s = 4,
    E = 5,
    F = 6,
    F_s = 7,
    G = 8,
    G_s = 9,
    A = 10,
    A_s = 11,
    B = 12
} Key;

typedef enum Scale {
    ANY_SCALE = 0,
    Major = 1,
    Minor = 2
} Scale;

typedef enum Octave {
    lower_lower = 1,
    lower = 2,
    middle = 3,
    upper = 4,
    upper_upper = 5
} Octave;

}
