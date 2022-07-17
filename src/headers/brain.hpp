#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <piksel/baseapp.hpp>
#include <iostream>
#include <cmath>
#include <random>
#include "headers/utils.hpp"


class Brain {
public:
    static uint inputSize;
    static uint hiddenSize;
    static uint outputSize;

    float **weightMatrix1;
    float **weightMatrix2;

    Brain();
    void randomInitializeBrain();
    float** initializeMatrix(uint rows, uint cols);
    float* dot(float** w, float* x, uint matRows, uint matCols);
    float* sigmoid(float* x, uint length);
    float* tanh(float* x, uint length);
    float* forward(float* input);
};

#endif /* BRAIN_HPP */