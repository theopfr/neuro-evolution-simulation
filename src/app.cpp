#include "app.hpp"
#include <iostream>
#include <vector>


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
        organisms.push_back(Organism(randomInt(spawnBorderOffset, this->width - spawnBorderOffset), randomInt(spawnBorderOffset, this->height - spawnBorderOffset)));
    }
    spawnFood(initialFoodAmount);
}


void App::draw(piksel::Graphics& g) {
    g.background(glm::vec4(0xf, 0xf, 0xf, 1.0f));
    
    iteration += 1;
    if (organisms.size() - 1 == 0) {
        std::cout << " average lifetime: " << totalLifeTimes / totalLifes << std::endl;
    }

    // iterate over organisms
    for (uint i=0; i<organisms.size() - 1; i++) {
        organisms.at(i).draw(g);
        organisms.at(i).move();
        organisms.at(i).update();

        // remove organism if dead and leave meat
        if (!organisms.at(i).alive) {
            totalLifes += 1;
            totalLifeTimes += organisms.at(i).currentLifeTime;

            Food foodFromOrganism = Food(organisms.at(i).position.x, organisms.at(i).position.y);
            foodFromOrganism.foodType = Meat;

            foods.push_back(foodFromOrganism);
            organisms.erase(organisms.begin() + i);
        }

        // check for collision with other organisms
        for (uint j=0; j<organisms.size() - 1; j++) {
            if (i == j) {
                continue;
            }
            if (distance(organisms.at(i).position, organisms.at(j).position) < (organisms.at(i).currentSize / 2 + organisms.at(j).currentSize / 2)) {
                if (organisms.at(i).horny && organisms.at(j).horny) {
                    organisms.push_back(Organism(organisms.at(i).position.x, organisms.at(i).position.y));
                    organisms.at(i).horniness -= 0.1;
                    organisms.at(j).horniness -= 0.1;
                    break;
                }
            }
        }

        // check for collision with food
        for (uint j=0; j<foods.size() - 1; j++) {
            if (distance(organisms.at(i).position, foods.at(j).position) < (organisms.at(i).currentSize / 2 + foods.at(j).size / 2)) {
                
                if (foods.at(j).foodType == Plant) {
                    organisms.at(i).energy += 0.2;
                }
                else if (foods.at(j).foodType == Meat) {
                    organisms.at(i).energy += 0.1;
                }

                foods.erase(foods.begin() + j);
                break;
            }
        }
    }

    // iterate over foods
    for (uint i=0; i<foods.size() - 1; i++) {
        foods.at(i).draw(g);
    }

    if (iteration % 20 == 0) {
        spawnFood(randomInt(1, 3));
    }

    handleanimationDelay(g);
}