
#include "headers/brain.hpp"
#include "headers/config.hpp"


Brain::Brain() {
    inputSize = config_inputSize;
    hiddenSize = config_hiddenSize;
    outputSize = config_outputSize;

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
    float* result = new float[matRows];
    for (uint i=0; i<matCols; i++) {
        result[i] = 0.0;
    }

    for (uint i=0; i<matRows; i++) {
        for (uint j=0; j<matCols; j++) {
            result[i] += w[i][j] * x[j];
        }
    }


    return result;
}

float* Brain::forward(std::vector<float> observation) {
    float* input = &observation[0];
    float* hiddenLayer = sigmoid(dot(weightMatrix1, input, hiddenSize, inputSize), hiddenSize);
    float* output = sigmoid(dot(weightMatrix2, hiddenLayer, outputSize, hiddenSize), outputSize);

    return output;
}

void Brain::mutate() {
    for (uint i=0; i<hiddenSize; i++) {
        for (uint j=0; j<inputSize; j++) {
            if (randomInt(0, 100) <= config_brainMutationProbability) {
                weightMatrix1[i][j] += randomFloat(-config_brainMutationFactor, config_brainMutationFactor);
            }
        }   
    }

    for (uint i=0; i<outputSize; i++) {
        for (uint j=0; j<hiddenSize; j++) {
            if (randomInt(0, 100) <= config_brainMutationProbability) {
                weightMatrix2[i][j] += randomFloat(-config_brainMutationFactor, config_brainMutationFactor);
            }
        }   
    }
}