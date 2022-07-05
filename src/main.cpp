#include "app.hpp"

int main() {
    uint width = 1100;
    uint height = 900;

    uint initialGenerationSize = 50;
    uint initialFoodAmount = 50;

    App app(width, height, initialGenerationSize, initialFoodAmount);
    app.start();
}