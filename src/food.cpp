#include <piksel/baseapp.hpp>
//#include "utils.cpp"
//#include "vector.cpp"
#include <iostream>


class Food {
public:
    Vector position;
    uint size = 6;

    // food type 0: plant, food type 1: meat
    uint foodType = 0;

    Food(uint startX, uint startY) {
        position.x = startX;
        position.y = startY;
    }

    void draw(piksel::Graphics& g) {
        if (foodType == 0) {
            g.fill(glm::vec4(0.0f, 0.0f, 0.0f, 0.33f));
        }
        else {
            g.fill(glm::vec4(1.0f, 0.0f, 0.0f, 0.33f));
        }
        g.noStroke();
        g.ellipse(position.x, position.y, size, size);
    }
};