#include <piksel/baseapp.hpp>
#include <cmath>


class Vector {
public:
    float x;
    float y;

    float length() {
        return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
    }
};