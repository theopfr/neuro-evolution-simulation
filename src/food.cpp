#include <piksel/baseapp.hpp>
//#include "utils.cpp"
//#include "vector.cpp"
#include <iostream>


class Food {
public:
    Vector position;
    uint size = 5;

    Food(uint startX, uint startY) {
        position.x = startX;
        position.y = startY;
    }

    void draw(piksel::Graphics& g) {
        g.fill(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
        g.ellipse(position.x, position.y, size, size);
    }
};