#ifndef APP_HPP
#define APP_HPP

#include <piksel/baseapp.hpp>
#include "organism.cpp"
#include "food.cpp"
#include <vector>


class App : public piksel::BaseApp {
public:

    uint width;
    uint height;
    uint spawnBorderOffset = 20;

    uint initialGenerationSize;
    uint initialFoodAmount;

    App(uint width, uint height, uint initialGenerationSize, uint initialFoodAmount) : piksel::BaseApp(width, height) {
        this->width = width;
        this->height = height;

        this->initialGenerationSize = initialGenerationSize;
        this->initialFoodAmount = initialFoodAmount;
    }

    void setup();
    void draw(piksel::Graphics& g);

    uint iteration;
    std::vector<Organism> organisms;
    std::vector<Food> foods;

    float totalLifeTimes = 0.0;
    int totalLifes = 0;

    void spawnInitialGeneration(uint amount) {
        for (uint i=0; i<amount; i++) {
            organisms.push_back(Organism(randomInt(spawnBorderOffset, this->width - spawnBorderOffset), randomInt(spawnBorderOffset, this->height - spawnBorderOffset)));
        }
    }

    void spawnFood(uint amount) {
        for (uint i=0; i<amount; i++) {
            foods.push_back(Food(randomInt(spawnBorderOffset, this->width - spawnBorderOffset), randomInt(spawnBorderOffset, this->height - spawnBorderOffset)));
        }
    }
};
#endif /* APP_HPP */


