#include <iostream>
#include <piksel/baseapp.hpp>
#include <cmath>

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

    uint universalMaxSightReach = config_universalMaxSightReach;
    uint universalMinSightReach = config_universalMinSightReach;

    uint mutationProbability = config_geneMutationProbability;
    float mutationFactor = config_geneMutationFactor;

    float diet;
    float maxSize;
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

    float getSightReach() {
        return convertToRange(sightReach, 0.0, 1.0, (float) universalMinSightReach, (float) universalMaxSightReach);
    }

    void mutate() {
        mutateGene(diet);
        mutateGene(maxSize);
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
        diet = randomFloat(0.0, 1.0);
        maxSize = randomFloat(0.0, 1.0);
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
    float initialEnergyLoss = config_energyLoss;
    float energyLoss = initialEnergyLoss;
    float wallDamage = config_wallDamage;
    uint currentLifeTime = 0;

    float maxEnergyGainByFood = config_maxEnergyGainByFood;
    float energyGainByPlant;
    float energyGainByMeat;

    bool breedable = false;
    float breedability = 0.0;
    float breedabilityRegeneration = config_breedabilityRegeneration;

    float recurrentState = randomFloat(0.0, 1.0);
    std::vector<float> observations;

    float timeToAttack = 0.0;

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

        g.fill(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        g.ellipse(position.x, position.y, currentSize, currentSize);

        if (timeToAttack > 0.0) {
            g.line(position.x, position.y, position.x + direction.x * (currentSize * 0.75), position.y + direction.y * (currentSize * 0.75));
        }
        else {
            g.line(position.x, position.y, position.x + direction.x * (currentSize * 0.5), position.y + direction.y * (currentSize * 0.5));
        }

        // draw two lines around the direction line to indicate the sight angle and reach
        float maxReachLineFactor = 1.25;
        float minReachLineFactor = 0.75;
        float reachLineFactor = convertToRange(genes.sightReach, 0.0, 1.0, minReachLineFactor, maxReachLineFactor);

        g.stroke(glm::vec4(0.0f, 0.0f, 0.0f, 0.2));
        g.line(position.x, position.y, position.x + sightEdge1.x * (currentSize * reachLineFactor), position.y + sightEdge1.y * (currentSize * reachLineFactor));
        g.line(position.x, position.y, position.x + sightEdge2.x * (currentSize * reachLineFactor), position.y + sightEdge2.y * (currentSize * reachLineFactor));

        g.ellipse(position.x + sightEdge1.x * (currentSize * reachLineFactor), position.y + sightEdge1.y * (currentSize * reachLineFactor), 3, 3);
        g.ellipse(position.x + sightEdge2.x * (currentSize * reachLineFactor), position.y + sightEdge2.y * (currentSize * reachLineFactor), 3, 3);
    }
    
    void setPosition(uint startX, uint startY) {
        position.x = startX;
        position.y = startY;
    }

    void setRandomGenes() {
        genes.setRandomGenes();
    }

    void inheritGenes(Organism parent1, Organism parent2) {
        if (parent1.energy > parent2.energy) {
            genes.diet = parent1.genes.diet;
            genes.maxSize = parent1.genes.maxSize;
            genes.sightReach = parent1.genes.sightReach;

            brain = parent1.brain;
        }
        else {
            genes.diet = parent2.genes.diet;
            genes.maxSize = parent2.genes.maxSize;
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
        // transform sensors vectors to real values
        sightEdge1.x = position.x + sightEdge1.x * genes.getSightReach();
        sightEdge1.y = position.y + sightEdge1.y * genes.getSightReach();
        sightEdge2.x = position.x + sightEdge2.x * genes.getSightReach();
        sightEdge2.y = position.y + sightEdge2.y * genes.getSightReach();

        // float sensorAngle = std::acos((sightEdge2.x * sightEdge1.x + sightEdge2.y * sightEdge1.y) / (sightEdge1.length() * sightEdge2.length()));

        // check for collision with other organism or distance to other organism
        bool organismFound = false;
        uint closestOrganismIndex = 0;
        float shortestDistanceToOrganism = genes.getSightReach() + currentSize / 2;

        for (uint i=0; i<organisms.size() - 1; i++) {
            if (&organisms.at(i) == this) {
                continue;
            }

            float distanceToOrganism = position.distance(organisms.at(i).position);
            if (distanceToOrganism <= shortestDistanceToOrganism) {
                organismFound = true;
                closestOrganismIndex = i;
                shortestDistanceToOrganism = distanceToOrganism;
            }
            
            // check for collision with other organism
            if (distanceToOrganism < (currentSize / 2 + organisms.at(i).currentSize / 2)) {
                if (breedable && organisms.at(i).breedable) {

                    Organism organism = Organism();
                    organism.setPosition(position.x, position.y);
                    organism.inheritGenes(*this, organisms.at(i));
                    organism.determineStats();

                    organisms.push_back(organism);

                    breedability = 0.0;
                    organisms.at(i).breedability = 0.0;
                }
            }
        }

        // check for collision with food or distance to food
        bool foodFound = false;
        uint closestFoodIndex = 0;
        float shortestDistanceToFood = genes.getSightReach();

        for (uint i=0; i<foods.size() - 1; i++) {
            float distanceToFood = position.distance(foods.at(i).position);

            if (distanceToFood < (currentSize / 2 + foods.at(i).size / 2)) {
                eat(foods.at(i).foodType);
                foods.erase(foods.begin() + i);
            }

            if (distanceToFood <= shortestDistanceToFood) {
                foodFound = true;
                closestFoodIndex = i;
                shortestDistanceToFood = distanceToFood;
            }
        }

        float organismDistance1 = 0.0;
        float organismDistance2 = 0.0;
        if (organismFound) {
            organismDistance1 = sightEdge1.distance(organisms.at(closestOrganismIndex).position) / genes.getSightReach();
            organismDistance2 = sightEdge2.distance(organisms.at(closestOrganismIndex).position) / genes.getSightReach();
        }

        float foodDistance1 = randomFloat(0.0, 1.0);
        float foodDistance2 = randomFloat(0.0, 1.0);
        if (foodFound) {
            foodDistance1 = sightEdge1.distance(foods.at(closestFoodIndex).position) / genes.getSightReach();
            foodDistance2 = sightEdge2.distance(foods.at(closestFoodIndex).position) / genes.getSightReach();
        }

        float wallDistanceX = randomFloat(0.0, 1.0);
        float wallDistanceY = randomFloat(0.0, 1.0);
        if (position.x - (currentSize / 2) <= 0 || position.y - (currentSize / 2) <= 0) {
            wallDistanceX = position.x / config_windowWidth;
            wallDistanceY = position.y / config_windowHeight;
        }

        observations = { foodDistance1, foodDistance2, wallDistanceX, wallDistanceY, recurrentState };
    }

    void update() {
        /* environmental causes */

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

        // check if organism is breedable, if not raise the breedability
        if (breedability < 1.0 || energy < 1.5) {
            breedability += breedabilityRegeneration;
            breedable = false;
        }
        else {
            breedable = true;
        }

        energyLoss = 1.0 / 400000.0 * currentLifeTime + initialEnergyLoss;
        std::cout << energyLoss << std::endl;

        /* actions */

        float* output = brain.forward(observations);

        float angle = output[0];

        /*float attackDecision = output[2];

        if (attackDecision > 0.5 && energy > 0.2 + 0.1) {
            energy -= 0.005;
            timeToAttack = 1.0;
        }
        if (timeToAttack > 0.0) {
            timeToAttack -= 0.5;
        }
        if (timeToAttack < 0.0) {
            timeToAttack = 0.0;
        }*/

        recurrentState = output[1];

        float sightAngleRange = 80.0;
        angle = convertToRange(angle, 0.0, 1.0, -sightAngleRange / 2.0, sightAngleRange / 2.0);

        // calculate turn angle
        angle = angle * (std::atan(1) * 4 / 180);
        direction.x = (direction.x * cos(angle)) - (direction.y * sin(angle));
        direction.y = (direction.x * sin(angle)) + (direction.y * cos(angle));
        direction.normalize();

        // update position
        position.x += direction.x * speed;
        position.y += direction.y * speed;

        float sightAngleLambda = sightAngleRange / 2.0 * (std::atan(1) * 4 / 180);
        sightEdge1.x = (direction.x * cos(sightAngleLambda)) - (direction.y * sin(sightAngleLambda));
        sightEdge1.y = (direction.x * sin(sightAngleLambda)) + (direction.y * cos(sightAngleLambda));

        sightEdge2.x = (direction.x * cos(-sightAngleLambda)) - (direction.y * sin(-sightAngleLambda));
        sightEdge2.y = (direction.x * sin(-sightAngleLambda)) + (direction.y * cos(-sightAngleLambda));

        sightEdge1.normalize();
        sightEdge2.normalize();

        // check if organism hits the wall and consume energy if it does
        if (position.x + direction.x < (currentSize / 2) || position.x + direction.x > config_windowWidth - (currentSize / 2) || position.y + direction.y < currentSize / 2 || position.y + direction.y > config_windowHeight - (currentSize / 2)) {
            direction.x *= -1.0;
            direction.y *= -1.0;
            energy -= wallDamage;
        }
    }
};