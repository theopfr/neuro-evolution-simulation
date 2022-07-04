#include <piksel/baseapp.hpp>
#include "utils.cpp"
#include "vector.cpp"
#include <iostream>



class Being {
public:

    Vector position;
    Vector direction;

    int angle;
    float wanderingStrength = 0.5;
    int maxSpeed = 2;

    bool alive = true;
    bool breedable = false;
    int breedableIn = 0;
    bool reproduced = false;

    uint maxSize = 20;
    float currentSize = 7.0;

    float energy = 1.0;
    float energyLoss = 0.002;

    uint currentLifeTime = 0;

    Being(uint startX, uint startY) {
        position.x = startX;
        position.y = startY;

        direction.x = -1;
        direction.y = 0;
    }

    void draw(piksel::Graphics& g) {
        g.ellipse(position.x, position.y, currentSize, currentSize);
        g.line(position.x, position.y, position.x + direction.x * currentSize / 4, position.y + direction.y * currentSize / 4);
    }

    void move() {
        energy -= energyLoss;
        currentLifeTime += 1;

        angle = randomInt(0, 360);
        direction.x = (direction.x + sin(angle * (std::atan(1) * 4 / 180)) * wanderingStrength);
        direction.y = (direction.y + cos(angle * (std::atan(1) * 4 / 180)) * wanderingStrength);

        float length = std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2));
        direction.x = direction.x / length * maxSpeed;
        direction.y = direction.y / length * maxSpeed;

        if (position.x + direction.x < 10 || position.x + direction.x > 1070) {
            alive = false;
        }
        else if (position.y + direction.y < 10 || position.y + direction.y > 890) {
            alive = false;
        }
        
        if (energy <= 0) {
            alive = false;
        }

        position.x += direction.x;
        position.y += direction.y;

        if (currentSize <= maxSize) {
            currentSize += (float) ((float) maxSize / 200.0);
        }

        if (currentLifeTime == 200 || currentLifeTime >= 200 && breedableIn == 0) {
            breedable = true;
        }

        if (reproduced) {
            breedableIn = 20;
            breedable = false;
            reproduced = false;
        }

        if (breedableIn > 0) {
            breedableIn -= 1;
        }

    }

};