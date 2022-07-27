#include "app.hpp"

int main() {
    uint width = 1300;
    uint height = 1000;

    uint initialGenerationSize = 75;
    uint initialFoodAmount = 175;

    App app(width, height, initialGenerationSize, initialFoodAmount);
    app.start();
}