#include <piksel/baseapp.hpp>
#include "utils.cpp"
#include "vector.cpp"
#include <iostream>



class Organism {
public:

    Vector position;
    Vector direction;

    int angle;
    float wanderingStrength = 0.5;
    int speed = 2;

    bool alive = true;
    
    bool horny = false;
    float horniness = 0;

    uint maxSize = 20;
    float currentSize = 7.0;

    float energy = 1.0;
    float energyLoss = 0.002;
    float wallDamage = 0.25;

    uint currentLifeTime = 0;

    Organism(uint startX, uint startY) {
        position.x = startX;
        position.y = startY;

        direction.x = 0;
        direction.y = 0;
    }

    void draw(piksel::Graphics& g) {
        if (energy < 0.1) {
            g.stroke(glm::vec4(0.9f, 0.0f, 0.0f, 0.5f));
        }
        else {
            g.stroke(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        }
        g.ellipse(position.x, position.y, currentSize, currentSize);
        g.line(position.x, position.y, position.x + direction.x * currentSize / 4, position.y + direction.y * currentSize / 4);
    }

    void move() {
        // consume energy, age organism by 1
        energy -= energyLoss;
        currentLifeTime += 1;

        // calculate new step
        angle = randomInt(0, 360);
        direction.x = (direction.x + sin(angle * (std::atan(1) * 4 / 180)) * wanderingStrength);
        direction.y = (direction.y + cos(angle * (std::atan(1) * 4 / 180)) * wanderingStrength);

        float length = std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2));
        direction.x = direction.x / length * speed;
        direction.y = direction.y / length * speed;

        position.x += direction.x;
        position.y += direction.y;

        // check if organism hits the wall and optionally consume energy
        if (position.x < 10 || position.x > 1090 || position.y < 10 || position.y > 890) {
            direction.x *= -1;
            direction.y *= -1;
            energy -= wallDamage;
        }
        
        // kill organism if its energy reaches 0
        if (energy <= 0) {
            alive = false;
        }

        // grow organism if it didn't reach full size yet
        if (currentSize <= maxSize) {
            currentSize += (float) ((float) maxSize / 200.0);
        }

        // check if organism is horny, if not raise the horniness
        if (horniness < 1.0) {
            horniness += 0.005;
            horny = false;
        }
        else {
            horny = true;
        }

    }

};