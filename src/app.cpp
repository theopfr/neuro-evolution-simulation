#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "app.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



void App::spawnFood(uint amount) {
    for (uint i=0; i<amount; i++) {
        foods.push_back(Food(randomInt(spawnBorderOffset, this->width - spawnBorderOffset), randomInt(spawnBorderOffset, this->height - spawnBorderOffset)));
    }
}


void App::keyPressed(int key) {
    // left arrow key click
    if (key == 262) {
        if (animationDelay < animationDelta) {
                animationDelay = 0;
        }
        else {
            animationDelay -= animationDelta;
        }
    }
    // right arrow key click
    else if (key == 263) {
        if (animationDelay > maxAnimationDelay - animationDelta) {
                animationDelay = maxAnimationDelay;
        }
        else {
            animationDelay += animationDelta;
        }
    }
}


void App::setup() {
    for (uint i=0; i<initialGenerationSize; i++) {
        Organism organism = Organism();
        organism.setPosition(randomInt(spawnBorderOffset, this->width - spawnBorderOffset), randomInt(spawnBorderOffset, this->height - spawnBorderOffset));
        organism.determineStats();
        organisms.push_back(organism);
    }
    spawnFood(initialFoodAmount);
}


void App::draw(piksel::Graphics& g) {
    g.background(glm::vec4(0xf, 0xf, 0xf, 1.0f));
    
    iteration += 1;
    //if (organisms.size() - 1 == 0) {
    //    std::cout << " average lifetime: " << totalLifeTimes / totalLifes << std::endl;
    //}    

    // iterate over organisms
    for (uint i=0; i<organisms.size() - 1; i++) {
        // remove organism if dead and leave meat
        if (!organisms.at(i).alive) {
            //totalLifes += 1;
            //totalLifeTimes += organisms.at(i).currentLifeTime;

            Food foodFromOrganism = Food(organisms.at(i).position.x, organisms.at(i).position.y);
            foodFromOrganism.foodType = Meat;
            foods.push_back(foodFromOrganism);

            organisms.erase(organisms.begin() + i);
        }
        else {
            organisms.at(i).draw(g);
            organisms.at(i).observe(g, organisms, foods);
            organisms.at(i).move();
            organisms.at(i).update();

            sizes.push_back(organisms.at(i).genes.getMaxSize());
            diets.push_back(organisms.at(i).genes.getDiet());
            sightAngles.push_back(organisms.at(i).genes.getSightAngle());
            sightReaches.push_back(organisms.at(i).genes.getSightReach());
        }
    }

    // iterate over foods
    for (uint i=0; i<foods.size() - 1; i++) {
        foods.at(i).draw(g);
    }

    spawnFood(randomFloat(0, 3));

    // log simulation data
    int amount = organisms.size();
    if (iteration % 25 == 0) {
        nlohmann::json outJson;
        std::ofstream output_file(simulationDataPath);

        outJson[std::to_string(iteration)]["organismAmount"] = amount;
        outJson[std::to_string(iteration)]["sizes"] = sizes;
        outJson[std::to_string(iteration)]["diets"] = sightReaches;
        outJson[std::to_string(iteration)]["sightAngles"] = sightAngles;
        outJson[std::to_string(iteration)]["sightReaches"] = sightReaches;

        output_file << std::setfill(' ') << std::setw(2) << outJson;

        output_file.close();
    }

    sizes.clear();
    sightAngles.clear();
    sightReaches.clear();


    handleAnimationDelay(g);
}