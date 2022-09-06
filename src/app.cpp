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



void App::spawnFood(uint amount, bool plantOnly) {
    for (uint i=0; i<amount; i++) {
        Food initialFood = Food(randomInt(spawnBorderOffset, this->width - spawnBorderOffset), randomInt(spawnBorderOffset, this->height - spawnBorderOffset), Plant);

        if (!plantOnly) {
            if (randomInt(0, 100) < 7) {
                initialFood.foodType = Meat;
            }
        }
        foods.push_back(initialFood);
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
        organism.determineDiet();
        organisms.push_back(organism);
    }
    spawnFood(initialFoodAmount, false);
}


void App::draw(piksel::Graphics& g) {
    g.background(glm::vec4(0xf, 0xf, 0xf, 1.0f));
    
    iteration += 1;

    // display over foods
    for (uint i=0; i<foods.size() - 1; i++) {
        foods.at(i).draw(g);
        foods.at(i).update();

        if (foods.at(i).disintegrated) {
            foods.erase(foods.begin() + i);
        }
    }

    // spawn new food
    uint interval = 1;
    /*if (iteration == 800) {
        interval += 1;
    }
    else if (iteration == 1200) {
        interval += 1;
    }*/
    /*if (iteration % interval == 0) {
        spawnFood(randomFloat(0, 2));
    }*/
    
    float foodSpawnChance = 80;
    if (iteration >= 5000) {
        foodSpawnChance = 65;
    }
    /*if (iteration >= 20000) {
        foodSpawnChance = 35;
    }*/
    if (randomInt(0, 100) < foodSpawnChance) {
        spawnFood(randomFloat(0, 3), true);
    }       

    // iterate over organisms
    for (uint i=0; i<organisms.size() - 1; i++) {
        // std::cout << i << " : ";

        // remove organism if dead and leave meat
        if (!organisms.at(i).alive) {
            Food foodFromOrganism = Food(organisms.at(i).position.x, organisms.at(i).position.y, Meat);
            foods.push_back(foodFromOrganism);

            organisms.erase(organisms.begin() + i);
        }
        else {
            organisms.at(i).draw(g);
            organisms.at(i).observe(g, organisms, foods);
            organisms.at(i).update();

            sizes.push_back(organisms.at(i).genes.getSize());
            speeds.push_back(organisms.at(i).genes.getSpeed());
            diets.push_back(organisms.at(i).genes.getDiet());
            sightReaches.push_back(organisms.at(i).genes.getSightReach());
            generations.push_back(organisms.at(i).generation);
            energies.push_back(organisms.at(i).energy);
        }
    }

    // log simulation data
    int amount = organisms.size();
    if (iteration % 25 == 0) {
        nlohmann::json outJson;
        std::ofstream output_file(simulationDataPath);

        outJson[std::to_string(iteration)]["organismAmount"] = amount;
        outJson[std::to_string(iteration)]["sizes"] = sizes;
        outJson[std::to_string(iteration)]["speeds"] = speeds;
        outJson[std::to_string(iteration)]["diets"] = diets;
        outJson[std::to_string(iteration)]["sightReaches"] = sightReaches;
        outJson[std::to_string(iteration)]["generations"] = generations;
        outJson[std::to_string(iteration)]["energies"] = energies;

        output_file << std::setfill(' ') << std::setw(2) << outJson;
        output_file.close();   
    }

    sizes.clear();
    speeds.clear();
    diets.clear();
    sightReaches.clear();
    generations.clear();
    energies.clear();

    handleAnimationDelay(g);
}