#ifndef APP_HPP
#define APP_HPP

#include <piksel/baseapp.hpp>
#include "organism.cpp"
#include "food.cpp"
#include "headers/brain.hpp"
#include <vector>
#include <unistd.h>
#include <random>
#include "headers/foodType.hpp"


class App : public piksel::BaseApp {
public:
    uint maxAnimationDelay = 50000;
    uint animationDelay = maxAnimationDelay;
    uint animationDelta = 2000;
    uint iteration = 0;

    uint width;
    uint height;
    uint spawnBorderOffset = 20;

    uint initialGenerationSize;
    uint initialFoodAmount;

    App(
        uint width, 
        uint height, 
        uint initialGenerationSize, 
        uint initialFoodAmount) : piksel::BaseApp(width, height) {

        this->width = width;
        this->height = height;

        this->initialGenerationSize = initialGenerationSize;
        this->initialFoodAmount = initialFoodAmount;
    }

    void setup();
    void draw(piksel::Graphics& g);
    void keyPressed(int key);
    void spawnFood(uint amount);

    std::vector<Organism> organisms;
    std::vector<Food> foods;

    float totalLifeTimes = 0.0;
    int totalLifes = 0;

    
    void handleAnimationDelay(piksel::Graphics& g) {
        // animation speed slide bar
        g.fill(glm::vec4(0.0f, 0.0f, 0.0f, 0.375f));
        g.rect(15, 15, 100, 13);

        // animation speed slider
        g.fill(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        g.stroke(glm::vec4(0.0f, 0.0f, 0.0f, 0.5f));
        g.strokeWeight(1.5);
        g.rect(15 + 100.0 * (1.0 - (float) animationDelay / 50000.0), 14, 6, 14);

        usleep(animationDelay);
    }

};
#endif /* APP_HPP */


