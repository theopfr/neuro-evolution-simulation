#include <iostream>
#include <piksel/baseapp.hpp>

#include "headers/utils.hpp"
#include "headers/vector.hpp"
#include "headers/foodType.hpp"
#include "headers/brain.hpp"
#include "headers/food.hpp"
#include "headers/config.hpp"



struct Genes {
    // universal bounderies for gene values
    uint universalMaxAdultSize = config_universalMaxAdultSize;
    uint universalMinAdultSize = config_universalMinAdultSize;

    uint universalMaxSightAngle = config_universalMaxSightAngle;
    uint universalMinSightAngle = config_universalMinSightAngle;

    uint universalMaxSightReach = config_universalMaxSightReach;
    uint universalMinSightReach = config_universalMinSightReach;

    uint mutationProbability = config_geneMutationProbability;
    float mutationFactor = config_geneMutationFactor;

    float diet;
    float maxSize;
    float sightAngle;
    float sightReach;

    Genes() {
        setRandomGenes();
    }

    // get functions which convert the raw 0-1 ranged genes into their true range
    float getDiet() {
        return diet;
    }

    float getMaxSize() {
        return convertToRange(maxSize, 0.0, 1.0, (float) universalMinAdultSize, (float) universalMaxAdultSize);
    }

    float getSightAngle() {
        return convertToRange(sightAngle, 0.0, 1.0, (float) universalMinSightAngle, (float) universalMaxSightAngle);
    }

    float getSightReach() {
        return convertToRange(sightReach, 0.0, 1.0, (float) universalMinSightReach, (float) universalMaxSightReach);
    }

    void mutate() {
        mutateGene(diet);
        mutateGene(maxSize);
        mutateGene(sightAngle);
        mutateGene(sightReach);
    }
 
    void mutateGene(float& gene) {
        if (randomInt(0, 100) <= mutationProbability) {
            gene += randomFloat(-mutationFactor, mutationFactor);
            if (gene < 0.0) {
                gene = 0.0;
            }
            else if (gene > 1.0) {
                gene = 1.0;
            }
        }
    }

    void setRandomGenes() {
        diet = randomFloat(0.0, 0.3);
        maxSize = randomFloat(0.0, 1.0);
        sightAngle = randomFloat(0.0, 1.0);
        sightReach = randomFloat(0.0, 1.0);
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
    float wanderingStrength = config_wanderingStrength;

    float speed;
    float minSpeed = config_minSpeed;
    float maxSpeed = config_maxSpeed;

    // current sight angle and reach
    Vector sightEdge1;
    Vector sightEdge2;

    // size values
    float currentSize = config_currentSize;

    bool alive = true;
    float energy = 1.0;
    float energyLoss = config_energyLoss;
    float wallDamage = config_wallDamage;
    uint currentLifeTime = 0;

    float maxEnergyGainByFood = config_maxEnergyGainByFood;
    float energyGainByPlant;
    float energyGainByMeat;

    bool horny = false;
    float horniness = 0.0;
    float horninessRegeneration = config_horninessRegeneration;

    float recurrentState = randomFloat(0.0, 1.0);
    std::vector<float> observations;

    Organism() {
        angle = randomInt(0, 360);
        direction.x = sin(angle * (std::atan(1) * 4 / 180));
        direction.y = cos(angle * (std::atan(1) * 4 / 180));
    }

    void draw(piksel::Graphics& g) {
        if (energy < 0.1) {
            g.stroke(glm::vec4(0.9f, 0.0f, 0.0f, 0.5f));
        }
        else {
            g.stroke(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        }

        g.ellipse(position.x, position.y, currentSize, currentSize);
        g.line(position.x, position.y, position.x + direction.x * (currentSize * 0.5), position.y + direction.y * (currentSize * 0.5));

        // draw two lines around the direction line to indicate the sight angle and reach
        float maxReachLineFactor = 1.25;
        float minReachLineFactor = 0.75;
        float reachLineFactor = convertToRange(genes.sightReach, 0.0, 1.0, minReachLineFactor, maxReachLineFactor);

        g.stroke(glm::vec4(0.0f, 0.0f, 0.0f, 0.2));
        g.line(position.x, position.y, position.x + sightEdge1.x * (currentSize * reachLineFactor), position.y + sightEdge1.y * (currentSize * reachLineFactor));
        g.line(position.x, position.y, position.x + sightEdge2.x * (currentSize * reachLineFactor), position.y + sightEdge2.y * (currentSize * reachLineFactor));
    }

    void setPosition(uint startX, uint startY) {
        position.x = startX;
        position.y = startY;
    }

    void setRandomGenes() {
        genes.setRandomGenes();
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

    void determineStats() {
        determineDiet();
        determineSpeed();
    }

    void determineDiet() {
        // 'diet' is a value between '0' and '1'. '0' means the organism is 100% herbivious, '1' means it's 100% carnivious.
        // Tending 100% towards one food type means it gains 'maxEnergyGainByFood' if it eats that type of food and '0' if it eats the other.
        // Everything in between means it gets ('diet' * 'maxEnergyGainByFood') when eating the preferred food type and
        // (1 - 'diet' * 'maxEnergyGainByFood') when eating the other

        if (genes.getDiet() < 0.5) {
            energyGainByPlant = (1 - genes.getDiet()) * maxEnergyGainByFood;
            energyGainByMeat = genes.getDiet() * maxEnergyGainByFood;
        }
        else {
            energyGainByMeat = genes.getDiet() * maxEnergyGainByFood;
            energyGainByPlant = (1 - genes.getDiet()) * maxEnergyGainByFood;
        }
    }

    void determineSpeed() {
        float additionalSpeedFactor = 1.0 - genes.maxSize;
        speed = minSpeed + (maxSpeed - minSpeed) * additionalSpeedFactor;
    }

    void eat(FoodType foodType) {
        if (foodType == Plant) {
            energy += energyGainByPlant;
        }
        else if (foodType == Meat) {
            energy += energyGainByMeat;
        }
    }

    void observe(piksel::Graphics& g, std::vector<Organism> &organisms, std::vector<Food> &foods) {
        // check for collision with other organisms
        for (uint i=0; i<organisms.size() - 1; i++) {
            if (&organisms.at(i) == this) {
                continue;
            }
            
            if (position.distance(organisms.at(i).position) < (currentSize / 2 + organisms.at(i).currentSize / 2)) {
                if (horny && organisms.at(i).horny && energy >= 1.5 && organisms.at(i).energy >= 1.5) {
                    //std::cout << this << " true" << std::endl;
                    Organism organism = Organism();
                    organism.setPosition(position.x, position.y);
                    organism.inheritGenes(*this, organisms.at(i));
                    organism.determineStats();

                    organisms.push_back(organism);

                    horniness -= 0.1;
                    organisms.at(i).horniness -= 0.1;
                    break;
                }
            }
        }

        // check for collision with food or distance to food        
        observations = { randomFloat(0.0, 1.0), randomFloat(0.0, 1.0), recurrentState };

        bool foodFound = false;
        uint clostestFoodIndex = 0;
        float shortestDistanceToFood = genes.getSightReach();

        for (uint i=0; i<foods.size() - 1; i++) {
            float distanceToFood = position.distance(foods.at(i).position);

            if (distanceToFood < (currentSize / 2 + foods.at(i).size / 2)) {
                eat(foods.at(i).foodType);
                foods.erase(foods.begin() + i);
            }

            if (distanceToFood <= shortestDistanceToFood) {
                foodFound = true;

                clostestFoodIndex = i;
                shortestDistanceToFood = distanceToFood;
                /*sightEdge1.x = position.x + sightEdge1.x * genes.getSightReach();
                sightEdge1.y = position.y + sightEdge1.y * genes.getSightReach();
                sightEdge2.x = position.x + sightEdge2.x * genes.getSightReach();
                sightEdge2.y = position.y + sightEdge2.y * genes.getSightReach();

                std::cout << "S1 " << "x: " << sightEdge1.x << " y: " << sightEdge1.y << std::endl;
                std::cout << "P " << "x: " << position.x << " y: " << position.y << std::endl;
                std::cout << sightEdge1.distance(foods.at(i).position) << " " << genes.getSightReach() << std::endl;
                std::cout << "\n\n------------------\n\n" << std::endl;

                observations = { sightEdge1.distance(foods.at(i).position) / genes.getSightReach(), sightEdge2.distance(foods.at(i).position) / genes.getSightReach(), recurrentState };
                */
            }
        }

        if (foodFound) {
            sightEdge1.x = position.x + sightEdge1.x * genes.getSightReach();
            sightEdge1.y = position.y + sightEdge1.y * genes.getSightReach();
            sightEdge2.x = position.x + sightEdge2.x * genes.getSightReach();
            sightEdge2.y = position.y + sightEdge2.y * genes.getSightReach();

            observations = { sightEdge1.distance(foods.at(clostestFoodIndex).position) / genes.getSightReach(), sightEdge2.distance(foods.at(clostestFoodIndex).position) / genes.getSightReach(), recurrentState };
        }

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
            horniness += horninessRegeneration;
            horny = false;
        }
        else {
            horny = true;
        }
    }

    void move() {
        //float observation[3] = { randomFloat(-2.0, 2.0), randomFloat(-2.0, 2.0), randomFloat(-2.0, 2.0), randomFloat(-2.0, 2.0), randomFloat(-2.0, 2.0) };
        float* output = brain.forward(observations);

        float angle = output[0];
        recurrentState = output[1];

        float maxAngleDelta = (genes.getSightAngle() / 2.0) * wanderingStrength;
        angle = convertToRange(angle, 0.0, 1.0, -maxAngleDelta, maxAngleDelta);

        // calculate turn angle
        angle = angle * (std::atan(1) * 4 / 180);
        direction.x = (direction.x * cos(angle)) - (direction.y * sin(angle));
        direction.y = (direction.x * sin(angle)) + (direction.y * cos(angle));
        direction.normalize();

        // update position
        position.x += direction.x * speed;
        position.y += direction.y * speed;

        float sightAngleLambda = genes.getSightAngle() / 2 * (std::atan(1) * 4 / 180);
        sightEdge1.x = (direction.x * cos(sightAngleLambda)) - (direction.y * sin(sightAngleLambda));
        sightEdge1.y = (direction.x * sin(sightAngleLambda)) + (direction.y * cos(sightAngleLambda));

        sightEdge2.x = (direction.x * cos(-sightAngleLambda)) - (direction.y * sin(-sightAngleLambda));
        sightEdge2.y = (direction.x * sin(-sightAngleLambda)) + (direction.y * cos(-sightAngleLambda));

        sightEdge1.normalize();
        sightEdge2.normalize();

        // check if organism hits the wall and consume energy if it does
        if (position.x + direction.x < (currentSize / 2) || position.x + direction.x > config_windowWidth - (currentSize / 2) || position.y + direction.y < currentSize / 2 || position.y + direction.y > config_windowHeight - (currentSize / 2)) {
            direction.x *= -1;
            direction.y *= -1;
            energy -= wallDamage;
        }
    }
};