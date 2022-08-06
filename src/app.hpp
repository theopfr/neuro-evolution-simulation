#ifndef APP_HPP
#define APP_HPP

#include <piksel/baseapp.hpp>
#include <vector>
#include <unistd.h>
#include <random>
#include "headers/foodType.hpp"
#include "organism.cpp"
#include "headers/food.hpp"
#include "headers/brain.hpp"

#include "headers/json.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream> 


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


    char simulationDataPath[32] = "../data/simulation_data.json";
    std::vector<float> sizes;
    std::vector<float> diets;
    std::vector<float> sightAngles;
    std::vector<float> sightReaches;

    App(
        uint width, 
        uint height, 
        uint initialGenerationSize, 
        uint initialFoodAmount) : piksel::BaseApp(width, height) {

        this->width = width;
        this->height = height;

        this->initialGenerationSize = initialGenerationSize;
        this->initialFoodAmount = initialFoodAmount;

        std::ofstream fw (simulationDataPath, std::ofstream::out);
        fw << "";
        fw.close();
    }

    void setup();
    void draw(piksel::Graphics& g);
    void keyPressed(int key);
    void spawnFood(uint amount);

    std::vector<Organism> organisms;
    std::vector<Food> foods;
    
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


