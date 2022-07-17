
#include "headers/brain.hpp"


uint Brain::inputSize = 5;
uint Brain::hiddenSize = 4;
uint Brain::outputSize = 3;

Brain::Brain() {
    randomInitializeBrain();
}

void Brain::randomInitializeBrain() {
    weightMatrix1 = initializeMatrix(hiddenSize, inputSize);
    weightMatrix2 = initializeMatrix(outputSize, hiddenSize);
}

float** Brain::initializeMatrix(uint rows, uint cols) {
    float** matrix;

    matrix = new float*[rows];
    for (uint i=0; i<rows; i++) {

        matrix[i] = new float[cols];
        for (uint j=0; j<cols; j++) {
            matrix[i][j] = randomFloat(-2, 2);
            //std::cout << matrix[i][j] << std::endl;
        }
    }
    return matrix;
}

float* Brain::sigmoid(float* x, uint length) {
    for (uint i=0; i<length; i++) {
        x[i] = 1.0 / (1.0 + std::exp(-x[i]));
    }
    return x;
}

float* Brain::tanh(float* x, uint length) {
    for (uint i=0; i<length; i++) {
        x[i] = (std::exp(x[i]) - std::exp(-x[i])) / (std::exp(x[i]) + std::exp(-x[i]));
    }
    return x;
}

float* Brain::dot(float** w, float* x, uint matRows, uint matCols) {
    float* result = new float[matCols];

    for (uint i=0; i<matRows; i++) {
        for (uint j=0; j<matCols; j++) {
            result[i] += w[i][j] * x[j];
        }
    }

    return result;
}

float* Brain::forward(float* input) {
    float* hiddenLayer = tanh(dot(weightMatrix1, input, hiddenSize, inputSize), hiddenSize);
    float* output = sigmoid(dot(weightMatrix2, hiddenLayer, outputSize, hiddenSize), outputSize);

    // std::cout << output[0] << " " << output[1] << " " << output[2] << std::endl;

    return output;
}
