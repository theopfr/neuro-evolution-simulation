#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fstream>
#include "headers/json.hpp"

// simulation window size configs
extern int config_windowWidth;
extern int config_windowHeight;

// simulation start configs
extern int config_initialGenerationSize;
extern int config_initialFoodAmount;

// gene configs
extern int config_universalMaxAdultSize;
extern int config_universalMinAdultSize;
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

// organism environmental configs
extern float config_wanderingStrength;
extern float config_minSpeed;
extern float config_maxSpeed;
extern float config_currentSize;
extern float config_energyLoss;
extern float config_wallDamage;
extern float config_maxEnergyGainByFood;
extern float config_breedabilityRegeneration;

#endif