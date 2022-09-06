#include <iostream>
#include <piksel/baseapp.hpp>
#include <cmath>
#include <assert.h>

#include "headers/utils.hpp"
#include "headers/vector.hpp"
#include "headers/foodType.hpp"
#include "headers/brain.hpp"
#include "headers/food.hpp"
#include "headers/config.hpp"


struct Genes {
    // universal bounderies for gene values
    int universalMaxAdultSize = config_universalMaxAdultSize;
    int universalMinAdultSize = config_universalMinAdultSize;

    int universalMaxSpeed = config_universalMaxSpeed;
    int universalMinSpeed = config_universalMinSpeed;

    int universalMaxSightReach = config_universalMaxSightReach;
    int universalMinSightReach = config_universalMinSightReach;

    int mutationProbability = config_geneMutationProbability;
    float mutationFactor = config_geneMutationFactor;

    float diet;
    float size;
    float speed;
    float sightReach;

    Genes() {
        setRandomGenes();
    }

    // get functions which convert the raw 0-1 ranged genes into their true range
    float getDiet() {
        return diet;
    }

    float getSize() {
        return (float) convertToRange(size, 0.0, 1.0, (float) universalMinAdultSize, (float) universalMaxAdultSize);
    }

    float getSpeed() {
        return (float) convertToRange(speed, 0.0, 1.0, (float) universalMinSpeed, (float) universalMaxSpeed);
    }

    float getSightReach() {
        return (float) convertToRange(sightReach, 0.0, 1.0, (float) universalMinSightReach, (float) universalMaxSightReach);
    }

    void mutate() {
        mutateGene(diet);
        mutateGene(size);
        mutateGene(speed);
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
        size = randomFloat(0.0, 1.0);
        speed = randomFloat(0.0, 1.0);
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
    float speed;

    // current sight angle and reach
    Vector sightEdge1;
    Vector sightEdge2;

    // size values
    float currentSize = config_initialSize;

    bool alive = true;
    float energy = 1.0;
    float energyLoss;
    float wallDamage = config_wallDamage;
    int currentLifeTime = 0;

    float maxEnergyGainByFood = config_maxEnergyGainByFood;
    float energyGainByPlant;
    float energyGainByMeat;
    float energyLossByRottenFood = config_energyLossByRottenFood;
    float openMouth = 0.0;

    int maxFoodStorage = 15;
    int currentFoodStorage = 0;
    int digestionIterations = 200;
    int digestionIterationCounter = 0;

    bool breedable = false;
    float breedability = 0.0;
    float breedabilityRegeneration = config_breedabilityRegeneration;

    float recurrentState = randomFloat(0.0, 1.0);
    std::vector<float> observations;

    uint generation = 0;

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

        /*if (currentFoodStorage == maxFoodStorage) {
            g.fill(glm::vec4(0.0f, 0.0f, 1.0f, 0.35f));
        }
        else {
            g.fill(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        }*/

        g.fill(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        g.ellipse(position.x, position.y, currentSize, currentSize);

        if (openMouth >= config_openMouthThreshold) {
            g.line(position.x, position.y, position.x + direction.x * (currentSize * 0.7), position.y + direction.y * (currentSize * 0.7));
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

        if (genes.diet > 0.5) {
            g.fill(glm::vec4(1.0f, 0.0f, 0.0f, 0.5));
        }
        g.ellipse(position.x + sightEdge1.x * (currentSize * reachLineFactor), position.y + sightEdge1.y * (currentSize * reachLineFactor), 3, 3);
        g.ellipse(position.x + sightEdge2.x * (currentSize * reachLineFactor), position.y + sightEdge2.y * (currentSize * reachLineFactor), 3, 3);
    }
    
    void setPosition(int startX, int startY) {
        position.x = startX;
        position.y = startY;
    }

    void setRandomGenes() {
        genes.setRandomGenes();
    }

    void inheritGenes(Organism parent1, Organism parent2) {
        // crossover diet
        if (randomInt(1, 2) == 1) { genes.diet = parent1.genes.diet; } 
        else { genes.diet = parent2.genes.diet; }
        // crossover size
        if (randomInt(1, 2) == 1) { genes.size = parent1.genes.size; } 
        else { genes.size = parent2.genes.size; }
        // crossover speed
        if (randomInt(1, 2) == 1) { genes.speed = parent1.genes.speed; } 
        else { genes.speed = parent2.genes.speed; }
        // crossover sight reach
        if (randomInt(1, 2) == 1) { genes.sightReach = parent1.genes.sightReach; } 
        else { genes.sightReach = parent2.genes.sightReach; }

        // inherit brain
        if (parent1.energy > parent2.energy) { brain = parent1.brain; } 
        else { brain = parent2.brain; }

        // mutate genes and brain
        genes.mutate();
        brain.mutate();
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

    void eat(FoodType foodType) {
        if (foodType == Plant) {
            if (currentFoodStorage == maxFoodStorage) {
                energy -= config_overFedEnergyLoss;
            }
            else {
                energy += energyGainByPlant;
                currentFoodStorage += 1;
            }
            // energy += energyGainByPlant;
        }
        else if (foodType == Meat) {
            if (currentFoodStorage == maxFoodStorage) {
                energy -= config_overFedEnergyLoss;
            }
            else {
                energy += energyGainByMeat;
                currentFoodStorage += 1;
            }
            //energy += energyGainByMeat;
        }
        else if (foodType == Rotten) {
            energy -= energyLossByRottenFood;
        }
    }

    void observe(piksel::Graphics& g, std::vector<Organism> &organisms, std::vector<Food> &foods) {
        // transform sensors vectors to real values
        sightEdge1.x = position.x + sightEdge1.x * genes.getSightReach();
        sightEdge1.y = position.y + sightEdge1.y * genes.getSightReach();
        sightEdge2.x = position.x + sightEdge2.x * genes.getSightReach();
        sightEdge2.y = position.y + sightEdge2.y * genes.getSightReach();

        // check for collision with other organism or distance to other organism
        bool organismFound = false;
        int closestOrganismIndex = 0;
        float shortestDistanceToOrganism = genes.getSightReach() + currentSize / 2;

        for (int i=0; i<organisms.size() - 1; i++) {
            if (&organisms.at(i) == this || organisms.at(i).alive == false) {
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
                
                // float attackWinProbability = convertToRange((genes.size - organisms.at(i).genes.size), -1.0, 1.0, 0.0, 1.0);
                // float energyGainByAttack = maxEnergyGainByFood * (2.0 - organisms.at(i).genes.diet) * (1 + organisms.at(i).genes.size);

                /*if (2 == 1 && attack > 0.95) {
                    float attackWinProbability = convertToRange((genes.size - organisms.at(i).genes.size), -1.0, 1.0, 0.0, 1.0);
                    float energyGainByAttack = maxEnergyGainByFood * (2.0 - organisms.at(i).genes.diet) * (1 + organisms.at(i).genes.size);

                    if (randomFloat(0.0, 1.0) < attackWinProbability) {
                        energy += energyGainByAttack;
                        organisms.at(i).alive = false;
                    }
                    else {
                        energy -= 0.3;
                    }

                }

                else if (2 == 1 && genes.diet > 0.5 && genes.size > organisms.at(i).genes.size && energy < 0.05) {
                    organisms.at(i).alive = false;
                    energy += energyGainByMeat * 2.0;//energyGainByMeat * (1.0 - organisms.at(i).genes.diet);
                }*/

                /*if (genes.getDiet() > 0.5 && organisms.at(i).genes.getDiet() < 0.5) {
                    float attackWinProbability = convertToRange((genes.size - organisms.at(i).genes.size), -1.0, 1.0, 0.0, 1.0);
                    if (randomFloat(0.0, 1.0) < attackWinProbability) {
                        energyGainByMeat = genes.getDiet() * maxEnergyGainByFood;
                        energy += energyGainByMeat;
                        //organisms.at(i).energy -= energyGainByMeat;
                    }
                    else {
                        energy -= 0.2;
                    }
                }*/

                if (breedable && organisms.at(i).breedable) {
                    /*int children = 1;
                    int twinChance = 5;
                    if (randomInt(0, 100) < twinChance) {
                        children = 2;
                    }

                    for (int c=0; c<children; c++) {
                        Organism organism = Organism();
                        organism.setPosition(position.x, position.y);
                        organism.inheritGenes(*this, organisms.at(i));
                        organism.determineDiet();
                        organism.generation = generation + 1;
                        organisms.push_back(organism);

                        breedability = 0.0;
                        organisms.at(i).breedability = 0.0;
                    }*/
                    Organism organism = Organism();
                    organism.setPosition(position.x, position.y);
                    organism.inheritGenes(*this, organisms.at(i));
                    organism.determineDiet();
                    organism.generation = generation + 1;
                    organisms.push_back(organism);

                    breedability = 0.0;
                    organisms.at(i).breedability = 0.0;
                }
                
            }
        }

        // check for collision with food or distance to food
        bool foodFound = false;
        int closestFoodIndex = 0;
        float shortestDistanceToFood = genes.getSightReach();

        for (int i=0; i<foods.size() - 1; i++) {
            float distanceToFood = position.distance(foods.at(i).position);

            if (distanceToFood < (currentSize / 2 + foods.at(i).size / 2) && openMouth >= config_openMouthThreshold) {
                eat(foods.at(i).foodType);
                foods.erase(foods.begin() + i);
            }
            else if (distanceToFood < (currentSize / 2 + foods.at(i).size / 2) && openMouth < config_openMouthThreshold) {
                std::cout << this << " closed" << std::endl;
            }

            if (distanceToFood <= shortestDistanceToFood) {
                foodFound = true;
                closestFoodIndex = i;
                shortestDistanceToFood = distanceToFood;
            }
        }

        float organismDistance1 = randomFloat(0.0, 1.0);
        float organismDistance2 = randomFloat(0.0, 1.0);
        float organismType = randomFloat(0.0, 1.0);
        if (organismFound) {
            organismDistance1 = sightEdge1.distance(organisms.at(closestOrganismIndex).position) / genes.getSightReach();
            organismDistance2 = sightEdge2.distance(organisms.at(closestOrganismIndex).position) / genes.getSightReach();

            if (organisms.at(closestOrganismIndex).genes.diet < 0.5) {
                organismType = 0.0;
            }
            else {
                organismType = 1.0;
            }
        }

        float foodDistance1 = randomFloat(0.0, 1.0);
        float foodDistance2 = randomFloat(0.0, 1.0);
        float foodType = randomFloat(0.0, 1.0);
        if (foodFound) {
            foodDistance1 = sightEdge1.distance(foods.at(closestFoodIndex).position) / genes.getSightReach();
            foodDistance2 = sightEdge2.distance(foods.at(closestFoodIndex).position) / genes.getSightReach();

            // food type encodings: Plant -> 0.0, Meat -> 0.5, Rotten -> 1.0;
            if (foods.at(closestFoodIndex).foodType == Plant) {
                foodType = 0.0;
            }
            else if (foods.at(closestFoodIndex).foodType == Meat) {
                foodType = 0.5;
            }
            else if (foods.at(closestFoodIndex).foodType == Rotten) {
                foodType = 1.0;
            }
        }

        // float wallDistanceX = randomFloat(0.0, 1.0);
        // float wallDistanceY = randomFloat(0.0, 1.0);

        /*float objectDistance1, objectDistance2, objectType;
        if ((foodDistance1 + foodDistance2) < (organismDistance1 + organismDistance2)) {
            objectDistance1 = foodDistance1;
            objectDistance2 = foodDistance2;
            if (foods.at(closestFoodIndex).foodType == Plant) {
                objectType = 0.0;
            }
            else if (foods.at(closestFoodIndex).foodType == Meat) {
                objectType = 0.25;
            }
            else if (foods.at(closestFoodIndex).foodType == Rotten) {
                objectType = 0.5;
            }
        }
        else {
            objectDistance1 = organismDistance1;
            objectDistance2 = organismDistance1;
            if (organismType == 0.0) {
                objectType = 0.75;
            }
            else if (organismType == 1.0) {
                objectType = 1.0;
            }
        }*/

        //observations = { objectDistance1, objectDistance2, objectType, (float) currentFoodStorage / (float) maxFoodStorage };
        observations = { foodDistance1, foodDistance2, foodType, (float) currentFoodStorage / (float) maxFoodStorage };
        // , organismDistance1, organismDistance2, organisms.at(closestOrganismIndex).genes.getSize()

        // observations = { foodDistance1, foodDistance2, foodType, organismDistance1, organismDistance2, organisms.at(closestOrganismIndex).genes.size }; //  , wallDistanceX, wallDistanceY, organismDistance1, organismDistance2, organismType
        // observations = { foodDistance1, foodDistance2, foodType, organismDistance1, organismDistance2 };

    }

    void update() {
        // consume energy
        energyLoss = config_initialEnergyLoss + 
                     convertToRange(genes.size, 0.0, 1.0, 0.0, config_maxEnergyLossBySize) + 
                     convertToRange(genes.speed, 0.0, 1.0, 0.0, config_maxEnergyLossBySpeed) + 
                     (config_energyLossIncrementionRate * currentLifeTime);

        energy -= energyLoss;

        if (currentFoodStorage == maxFoodStorage) {
            digestionIterationCounter += 1;
        }
        
        if (digestionIterationCounter == digestionIterations) {
            currentFoodStorage = 0;
            digestionIterationCounter = 0;
        }
        
        // kill organism if its energy reaches 0
        if (energy <= 0 || currentLifeTime > 10000) {
            alive = false;
        }

        // grow organism if it didn't reach full size yet
        if (currentSize < genes.getSize()) {
            currentSize += (float) ((float) genes.getSize() / 200.0);
            energy += energyLoss;
        }

        // check if organism is breedable, if not raise the breedability
        if (breedability < 1.0 || energy < config_minEnergyToBreed) {
            if (currentSize >= genes.getSize()) {
                breedability += breedabilityRegeneration;
            }
            breedable = false;
        }
        else if (breedability >= 1.0 && currentSize >= genes.getSize()) {
            breedable = true;
        }

        /* actions */
        float* output = brain.forward(observations);
        angle = output[0];
        openMouth = output[1];

        float turnAngleRange = (float) config_maxTurnAngle;
        angle = convertToRange(angle, 0.0, 1.0, -turnAngleRange / 2.0, turnAngleRange / 2.0);

        // calculate turn angle
        angle = angle * (std::atan(1) * 4 / 180);
        direction.x = (direction.x * cos(angle)) - (direction.y * sin(angle));
        direction.y = (direction.x * sin(angle)) + (direction.y * cos(angle));
        direction.normalize();

        // update position
        position.x += direction.x * genes.getSpeed();
        position.y += direction.y * genes.getSpeed();

        float sightAngleLambda = turnAngleRange / 2.0 * (std::atan(1) * 4 / 180);
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

        currentLifeTime += 1;
    }
};