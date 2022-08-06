#include "app.hpp"
#include "headers/config.hpp"
#include <unistd.h>

int main() {
    App app(config_windowWidth, config_windowHeight, config_initialGenerationSize, config_initialFoodAmount);
    app.start();
}