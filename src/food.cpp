#include <piksel/baseapp.hpp>
#include <iostream>
#include <headers/vector.hpp>
#include "headers/foodType.hpp"
#include "headers/food.hpp"




Food::Food(uint startX, uint startY, FoodType foodType) {
    position.x = startX;
    position.y = startY;

    this->foodType = foodType;
}

void Food::draw(piksel::Graphics& g) {
    if (this->foodType == Plant) {
        g.fill(glm::vec4(0.0f, 0.0f, 0.0f, 0.33f));
    }
    else if (this->foodType == Meat) {
        g.fill(glm::vec4(1.0f, 0.0f, 0.0f, 0.33f));
    }
    else if (this->foodType == Rotten) {
        g.fill(glm::vec4(0.0f, 0.5f, 0.0f, 0.33f));
    }
    g.noStroke();
    g.ellipse(position.x, position.y, size, size);
}

void Food::update() {
    if (currentLifeTime % 500 == 0) {
        uint rottenChance = 4;
        if (randomInt(0, 100) <= rottenChance) {
            this->foodType = Rotten;
        }
    }
    else if (foodType == Rotten && currentLifeTime == 1000) {
        disintegrated = true;
    }

    currentLifeTime += 1;
}

