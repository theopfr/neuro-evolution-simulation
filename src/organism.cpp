#include <iostream>
#include <piksel/baseapp.hpp>

#include "headers/utils.hpp"
#include "headers/vector.hpp"
#include "headers/foodType.hpp"
#include "headers/brain.hpp"


class Organism {
public:

    Vector position;
    Vector direction;

    float angle = randomInt(0, 360);
    float wanderingStrength = 0.2;
    int speed = 2;
    Vector sightEdge1;
    Vector sightEdge2;
    uint sightAngle = 120;
    uint sightReach = 25;
    float maxAngleDelta = ((float) sightAngle / 2.0) * wanderingStrength;

    uint maxSize = 20;
    float currentSize = 7.0;

    bool alive = true;
    float energy = 1.0;
    float energyLoss = 0.002;
    float wallDamage = 0.25;
    uint currentLifeTime = 0;

    float maxEnergyGainByFood = 0.5;
    float diet = 0.2;
    float energyGainByPlant;
    float energyGainByMeat;

    bool horny = false;
    float horniness = 0;

    Brain brain = Brain();

    Organism(uint startX, uint startY) {
        position.x = startX;
        position.y = startY;

        angle = randomInt(0, 360);
        direction.x = sin(angle * (std::atan(1) * 4 / 180));
        direction.y = cos(angle * (std::atan(1) * 4 / 180));

        determineDiet();    
    }

    void draw(piksel::Graphics& g) {
        if (energy < 0.1) {
            g.stroke(glm::vec4(0.9f, 0.0f, 0.0f, 0.5f));
        }
        else {
            g.stroke(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        }
        g.ellipse(position.x, position.y, currentSize, currentSize);
        g.line(position.x, position.y, position.x + direction.x * (currentSize / 4), position.y + direction.y * (currentSize / 4));
        g.point(direction.x, direction.y);

        g.stroke(glm::vec4(0.0f, 0.0f, 0.0f, 0.2));
        g.line(position.x, position.y, position.x + sightEdge1.x * (currentSize / 1.75), position.y + sightEdge1.y * (currentSize / 1.75));
        g.line(position.x, position.y, position.x + sightEdge2.x * (currentSize / 1.75), position.y + sightEdge2.y * (currentSize / 1.75));
    }

    void determineDiet() {
        // 'diet' is value between '0' and '1'. '0' means the organism is 100% herbivious, '1' means it's 100% carnivious.
        // Tending 100% towads one food type means it gains 'maxEnergyGainByFood' if it eats that type of food and '0' if it eats the other.
        // Everything in between means it gets ('diet' * 'maxEnergyGainByFood') when eating the preferred food type and
        // (1 - 'diet' * 'maxEnergyGainByFood') when eating the other

        float energyGainByMajor = diet * maxEnergyGainByFood;
        float energyGainByMinor = maxEnergyGainByFood - energyGainByMajor;

        if (diet < 0.5) {
            energyGainByPlant = energyGainByMajor;
            energyGainByMeat = energyGainByMinor;
        }
        else {
            energyGainByMeat = energyGainByMajor;
            energyGainByPlant = energyGainByMinor;
        }
    }

    void eat(FoodType foodType) {
        if (foodType == Plant) {
            energy += energyGainByPlant;
        }
        else if (foodType == Meat) {
            energy += energyGainByMeat;
        }
    }

    void observe() {
        
    }

    void update() {
        // consume energy, age organism by 1
        energy -= energyLoss;
        currentLifeTime += 1;

        // kill organism if its energy reaches 0
        if (energy <= 0) {
            alive = false;
        }

        // grow organism if it didn't reach full size yet
        if (currentSize < maxSize) {
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

    void move() {
        //angle = randomInt(0, 360);

        float observation[5] = { randomFloat(-2.0, 2.0), randomFloat(-2.0, 2.0), randomFloat(-2.0, 2.0), randomFloat(-2.0, 2.0), randomFloat(-2.0, 2.0) };
        float* output = brain.forward(observation);
        angle = ((output[0] - 0.0) / (1.0 - 0.0)) * (maxAngleDelta - (-maxAngleDelta)) + (-maxAngleDelta);
        
        // calculate turn angle
        angle = angle * (std::atan(1) * 4 / 180);
        direction.x = (direction.x * cos(angle)) - (direction.y * sin(angle));
        direction.y = (direction.x * sin(angle)) + (direction.y * cos(angle));

        // normalize next step
        float length = std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2));
        direction.x = direction.x / length * speed;
        direction.y = direction.y / length * speed;

        // update position
        position.x += direction.x;
        position.y += direction.y;

        // calculate two lines to indicate the view angle
        float sightAngleLambda = sightAngle / 2 * (std::atan(1) * 4 / 180);
        sightEdge1.x = (direction.x * cos(sightAngleLambda)) - (direction.y * sin(sightAngleLambda));
        sightEdge1.y = (direction.x * sin(sightAngleLambda)) + (direction.y * cos(sightAngleLambda));

        sightEdge2.x = (direction.x * cos(-sightAngleLambda)) - (direction.y * sin(-sightAngleLambda));
        sightEdge2.y = (direction.x * sin(-sightAngleLambda)) + (direction.y * cos(-sightAngleLambda));

        // check if organism hits the wall and consume energy if it does
        if (position.x + direction.x < 20 || position.x + direction.x > 1070 || position.y + direction.y < 20 || position.y + direction.y > 880) {
            direction.x *= -1;
            direction.y *= -1;
            energy -= wallDamage;
        }
    }

    void crossover() {

    }

};



// std::cout << maxAngleDelta << " " << output[0] << " " << angle << std::endl;

/*if (angleDelta < 0.0) {
    angleDelta = 360.0 + angleDelta;
}

angle = angleDelta;//randomInt(-20, 20);
*/

//direction.x = (direction.x + sin(angle * (std::atan(1) * 4 / 180)) * wanderingStrength);
//direction.y = (direction.y + cos(angle * (std::atan(1) * 4 / 180)) * wanderingStrength);

/*float length = std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2));
direction.x = direction.x / length * speed;
direction.y = direction.y / length * speed;

position.x += direction.x;
position.y += direction.y;*/

//std::cout << " " << direction.x << " " << direction.y << std::endl;