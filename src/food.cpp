#include <piksel/baseapp.hpp>
#include <iostream>
#include <headers/vector.hpp>
#include "headers/foodType.hpp"
#include "headers/food.hpp"




Food::Food(uint startX, uint startY) {
    position.x = startX;
    position.y = startY;
}

void Food::draw(piksel::Graphics& g) {
    if (foodType == 0) {
        g.fill(glm::vec4(0.0f, 0.0f, 0.0f, 0.33f));
    }
    else {
        g.fill(glm::vec4(1.0f, 0.0f, 0.0f, 0.33f));
    }
    g.noStroke();
    g.ellipse(position.x, position.y, size, size);
}
