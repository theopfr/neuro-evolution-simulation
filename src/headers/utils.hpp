#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <random>


inline int randomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(min, max);

    return distribution(gen);
}

inline float randomFloat(float min, float max) {
    return ((float) randomInt(min * 1000.0, max * 1000.0)) / 1000.0;
}

inline float convertToRange(float value, float oldMin, float oldMax, float newMin, float newMax) {
    return ((value - oldMin) / (oldMax - oldMin)) * (newMax - newMin) + newMin;
}



#endif
