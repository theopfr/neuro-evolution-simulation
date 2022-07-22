#include <iostream>
#include <piksel/baseapp.hpp>

#include "headers/utils.hpp"
#include "headers/vector.hpp"
#include "headers/foodType.hpp"
#include "headers/brain.hpp"


struct Genes {
    // universal bounderies for gene values
    uint universalMaxAdultSize = 20;
    uint universalMinAdultSize = 15;

    uint universalMaxSightAngle = 180;
    uint universalMinSightAngle = 100;

    uint universalMaxSightReach = 40;
    uint universalMinSightReach = 20;

    float diet;
    float maxSize;
    float sightAngle;
    float sightReach;

    float convertToRange(float value, int min, int max) {
        return ((value - 0.0) / (1.0 - 0.0)) * ((float) max - ((float) min)) + ((float) min);
    }

    float getDiet() {
        return diet;
    }

    float getMaxSize() {
        std::cout << convertToRange(maxSize, universalMinAdultSize, universalMaxAdultSize) << std::endl;
        return convertToRange(maxSize, universalMinAdultSize, universalMaxAdultSize);
    }

    float getSightAngle() {
        return convertToRange(sightAngle, universalMinSightAngle, universalMaxSightAngle);
    }

    float getSightReach() {
        return convertToRange(sightReach, universalMinSightReach, universalMaxSightReach);
    }
 
    void mutate() {
        return;
    }
};



class Organism {
public:
    // genes
    Genes genes = Genes();

    // brain
    Brain brain = Brain();

    // position
    Vector position;
    Vector direction;

    // current direction and speed
    float angle;
    float speed = 2.0;
    float wanderingStrength = 0.2;
    
    // current sight angle and reach
    Vector sightEdge1;
    Vector sightEdge2;

    // size values
    float currentSize = 7.0;

    bool alive = true;
    float energy = 1.0;
    float energyLoss = 0.002;
    float wallDamage = 0.25;
    uint currentLifeTime = 0;

    float maxEnergyGainByFood = 0.5;
    float energyGainByPlant;
    float energyGainByMeat;

    bool horny = false;
    float horniness = 0;

    Organism() {
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

        g.stroke(glm::vec4(0.0f, 0.0f, 0.0f, 0.2));
        g.line(position.x, position.y, position.x + sightEdge1.x * (currentSize / 1.75), position.y + sightEdge1.y * (currentSize / 1.75));
        g.line(position.x, position.y, position.x + sightEdge2.x * (currentSize / 1.75), position.y + sightEdge2.y * (currentSize / 1.75));
    }

    void setPosition(uint startX, uint startY) {
        position.x = startX;
        position.y = startY;
    }

    void setRandomGenes() {
        genes.diet = randomFloat(0.0, 0.5);
        genes.maxSize = randomFloat(0.0, 1.0);
        genes.sightAngle = randomFloat(0.0, 1.0);
        genes.sightReach = randomFloat(0.0, 1.0);
    }

    void inheritGenes(Organism parent1, Organism parent2) {
        if (parent1.currentLifeTime > parent2.currentLifeTime) {
            genes.diet = parent1.genes.diet;
            genes.maxSize = parent1.genes.maxSize;
            genes.sightAngle = parent1.genes.sightAngle;
            genes.sightReach = parent1.genes.sightReach;

            brain = parent1.brain;
        }
        else {
            genes.diet = parent2.genes.diet;
            genes.maxSize = parent2.genes.maxSize;
            genes.sightAngle = parent2.genes.sightAngle;
            genes.sightReach = parent2.genes.sightReach;

            brain = parent2.brain;
        }
        
        genes.mutate();
        brain.mutate();
    }

    void determineDiet() {
        // 'diet' is value between '0' and '1'. '0' means the organism is 100% herbivious, '1' means it's 100% carnivious.
        // Tending 100% towads one food type means it gains 'maxEnergyGainByFood' if it eats that type of food and '0' if it eats the other.
        // Everything in between means it gets ('diet' * 'maxEnergyGainByFood') when eating the preferred food type and
        // (1 - 'diet' * 'maxEnergyGainByFood') when eating the other

        float energyGainByMajor = genes.getDiet() * maxEnergyGainByFood;
        float energyGainByMinor = maxEnergyGainByFood - energyGainByMajor;

        if (genes.getDiet() < 0.5) {
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
        if (currentSize < genes.getMaxSize()) {
            currentSize += (float) ((float) genes.getMaxSize() / 200.0);
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

        float maxAngleDelta = (genes.getSightAngle() / 2.0) * wanderingStrength;
        angle = ((output[0] - 0.0) / (1.0 - 0.0)) * (maxAngleDelta - (-maxAngleDelta)) + (-maxAngleDelta); //randomFloat(0.0, 1.0);

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
        float sightAngleLambda = genes.getSightAngle() / 2 * (std::atan(1) * 4 / 180);
        sightEdge1.x = (direction.x * cos(sightAngleLambda)) - (direction.y * sin(sightAngleLambda));
        sightEdge1.y = (direction.x * sin(sightAngleLambda)) + (direction.y * cos(sightAngleLambda));

        sightEdge2.x = (direction.x * cos(-sightAngleLambda)) - (direction.y * sin(-sightAngleLambda));
        sightEdge2.y = (direction.x * sin(-sightAngleLambda)) + (direction.y * cos(-sightAngleLambda));

        // check if organism hits the wall and consume energy if it does
        if (position.x + direction.x < 20 || position.x + direction.x > 1270 || position.y + direction.y < 20 || position.y + direction.y > 970) {
            direction.x *= -1;
            direction.y *= -1;
            energy -= wallDamage;
        }
    }

    void crossover() {

    }

};