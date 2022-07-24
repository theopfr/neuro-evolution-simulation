#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <cmath>


class Vector {
public:
    float x;
    float y;

    float length();
    void normalize();
};
#endif