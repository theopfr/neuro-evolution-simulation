#include <iostream>
#include <random>


inline int randomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(min, max);

    return distribution(gen);
}

inline float randomFloat() {
    return (float) randomInt(-100, 100) / 100.0;
}

