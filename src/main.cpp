#include "app.hpp"
#include <unistd.h>

int main() {
    uint width = 1300;
    uint height = 1000;

    uint initialGenerationSize = 250;
    uint initialFoodAmount = 175;

    App app(width, height, initialGenerationSize, initialFoodAmount);
    app.start();
}