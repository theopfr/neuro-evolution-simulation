#include "app.hpp"

int main() {
    uint width = 1300;
    uint height = 1000;

    uint initialGenerationSize = 100;
    uint initialFoodAmount = 200;

    App app(width, height, initialGenerationSize, initialFoodAmount);
    app.start();
}