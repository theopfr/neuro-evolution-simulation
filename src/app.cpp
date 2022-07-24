#include "app.hpp"
#include <iostream>
#include <vector>
#include <algorithm>


float distance(Vector a, Vector b) {
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}


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
        organisms.at(i).move();
        organisms.at(i).update();

        // std::cout << i << ": " << organisms.at(i).genes.getMaxSize() << " " << organisms.at(i).speed << std::endl;;

        // check for collision with other organisms
        for (uint j=0; j<organisms.size() - 1; j++) {
            if (i == j) {
                continue;
            }
            if (distance(organisms.at(i).position, organisms.at(j).position) < (organisms.at(i).currentSize / 2 + organisms.at(j).currentSize / 2)) {
                if (organisms.at(i).horny && organisms.at(j).horny) {
                    Organism organism = Organism();
                    organism.setPosition(organisms.at(i).position.x, organisms.at(i).position.y);
                    organism.inheritGenes(organisms.at(i), organisms.at(j));
                    organism.determineStats();

                    organisms.push_back(organism);

                    organisms.at(i).horniness -= 0.1;
                    organisms.at(j).horniness -= 0.1;
                    break;
                }
            }
        }

        // check for collision with food
        for (uint j=0; j<foods.size() - 1; j++) {
            if (distance(organisms.at(i).position, foods.at(j).position) < (organisms.at(i).currentSize / 2 + foods.at(j).size / 2)) {
                organisms.at(i).eat(foods.at(j).foodType);
                foods.erase(foods.begin() + j);
                break;
            }
        }
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