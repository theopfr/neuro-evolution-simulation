#ifndef APP_HPP
#define APP_HPP

#include <piksel/baseapp.hpp>
#include "organism.cpp"
#include "food.cpp"
#include <vector>
#include <unistd.h>


class App : public piksel::BaseApp {
public:

    uint animationDelay = 50000;

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

    void handleanimationDelay(piksel::Graphics& g) {
        g.fill(glm::vec4(0.0f, 0.0f, 0.0f, 0.375f));
        g.rect(15, 15, 100, 13);

        g.fill(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        g.stroke(glm::vec4(0.0f, 0.0f, 0.0f, 0.5f));
        g.strokeWeight(1.5);
        g.rect(15 + 100.0 * (1.0 - (float) animationDelay / 50000.0), 14, 6, 14);


        usleep(animationDelay);
    }
};
#endif /* APP_HPP */


