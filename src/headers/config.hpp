#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fstream>
#include "headers/json.hpp"

// simulation configs
extern int config_windowWidth;
extern int config_windowHeight;
extern int config_initialGenerationSize;
extern int config_initialFoodAmount;

// simulation fixed rule configs
extern float config_initialSize;
extern float config_wallDamage;
extern float config_maxEnergyGainByFood;
extern float config_breedabilityRegeneration;
extern float config_minEnergyToBreed;
extern float config_openMouthThreshold;
extern float config_maxTurnAngle;

// organism energy configs
extern float config_initialEnergyLoss;
extern float config_energyLossByRottenFood;
extern float config_energyLossIncrementionRate;
extern float config_maxEnergyLossBySpeed;
extern float config_maxEnergyLossBySize;
extern float config_overFedEnergyLoss;
extern float config_maxEnergyCapacity;

// gene configs
extern int config_universalMaxAdultSize;
extern int config_universalMinAdultSize;
extern int config_universalMaxSpeed;
extern int config_universalMinSpeed;
extern int config_universalMaxSightReach;
extern int config_universalMinSightReach;
extern int config_geneMutationProbability;
extern float config_geneMutationFactor;

// brain configs
extern int config_inputSize;
extern int config_hiddenSize;
extern int config_outputSize;
extern int config_brainMutationProbability;
extern float config_brainMutationFactor;



#endif