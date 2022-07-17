#include "headers/vector.hpp"
#include <cmath>


float Vector::length() {
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}