#include <unistd.h>
#include "app.hpp"
#include <iostream>

#include <vector>


float distance(Vector a, Vector b) {
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}


void App::setup() {
    spawnInitialGeneration(50);
    spawnInitialFood(200);

    iteration = 0;
}


void App::draw(piksel::Graphics& g) {
    g.background(glm::vec4(0xf, 0xf, 0xf, 1.0f));

    iteration += 1;
    if (iteration % 100 == 0) {
        std::cout << "average lifetime: " << totalLifeTimes / totalLifes << std::endl;
    }

    // iterate over beings
    for (uint i=0; i<beings.size() - 1; i++) {
        beings.at(i).move();
        beings.at(i).draw(g);

        // remove being if dead
        if (!beings.at(i).alive) {
            totalLifes += 1;
            totalLifeTimes += beings.at(i).currentLifeTime;
            beings.erase(beings.begin() + i);
        }

        // check for collision with other beings
        for (uint j=0; j<beings.size() - 1; j++) {
            if (i == j) {
                continue;
            }
            if (distance(beings.at(i).position, beings.at(j).position) < (beings.at(i).currentSize / 2 + beings.at(j).currentSize / 2) &&
                beings.at(i).breedable && beings.at(j).breedable) {

                beings.push_back(Being(beings.at(i).position.x, beings.at(i).position.y));
                beings.at(i).reproduced = true;
                beings.at(j).reproduced = true;
                break;
            }
        }

        // check for collision with food
        for (uint j=0; j<foods.size() - 1; j++) {
            if (distance(beings.at(i).position, foods.at(j).position) < (beings.at(i).currentSize / 2 + foods.at(j).size / 2)) {
                foods.erase(foods.begin() + j);

                beings.at(i).energy += 0.2;

                break;
            }
        }
    }

    // iterate over foods
    for (uint i=0; i<foods.size() - 1; i++) {
        foods.at(i).draw(g);
    }

    usleep(50000);
}