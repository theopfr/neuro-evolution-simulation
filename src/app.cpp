#include "app.hpp"
#include <iostream>
#include <vector>
#include <algorithm>



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
    if (organisms.size() - 1 == 0) {
        std::cout << " average lifetime: " << totalLifeTimes / totalLifes << std::endl;
    }
    
    // std::cout << organisms.size() << std::endl;

    // iterate over organisms
    for (uint i=0; i<organisms.size() - 1; i++) {

        //std::cout << i << ": ";
        // remove organism if dead and leave meat
        if (!organisms.at(i).alive) {
            totalLifes += 1;
            totalLifeTimes += organisms.at(i).currentLifeTime;

            Food foodFromOrganism = Food(organisms.at(i).position.x, organisms.at(i).position.y);
            foodFromOrganism.foodType = Meat;
            foods.push_back(foodFromOrganism);

            organisms.erase(organisms.begin() + i);

            continue;
        }

        organisms.at(i).draw(g);
        organisms.at(i).observe(g, organisms, foods);
        organisms.at(i).move();
        organisms.at(i).update();
    }

    // iterate over foods
    for (uint i=0; i<foods.size() - 1; i++) {
        foods.at(i).draw(g);
    }


    // create new food
    //if (foods.size() < initialFoodAmount) {
    spawnFood(randomFloat(0, 3));
    //}

    handleAnimationDelay(g);
}