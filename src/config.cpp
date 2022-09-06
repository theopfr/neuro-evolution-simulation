#include "headers/config.hpp"

std::ifstream f("../src/config.json");
nlohmann::json data = nlohmann::json::parse(f);


int config_windowWidth = data["windowWidth"];
int config_windowHeight = data["windowHeight"];
int config_initialGenerationSize = data["initialGenerationSize"];
int config_initialFoodAmount = data["initialFoodAmount"];

float config_initialSize = data["initialSize"];
float config_wallDamage = data["wallDamage"];
float config_maxEnergyGainByFood = data["maxEnergyGainByFood"];
float config_breedabilityRegeneration = data["breedabilityRegeneration"];
float config_minEnergyToBreed = data["minEnergyToBreed"];
float config_openMouthThreshold = data["openMouthThreshold"];
float config_maxTurnAngle = data["maxTurnAngle"];

float config_initialEnergyLoss = data["initialEnergyLoss"];
float config_energyLossByRottenFood = data["energyLossByRottenFood"];
float config_energyLossIncrementionRate;
float config_maxEnergyLossBySpeed = data["maxEnergyLossBySpeed"];
float config_maxEnergyLossBySize = data["maxEnergyLossBySize"];
float config_overFedEnergyLoss = data["overFedEnergyLoss"];
float config_maxEnergyCapacity = data["maxEnergyCapacity"];

int config_universalMaxAdultSize = data["universalMaxAdultSize"];
int config_universalMinAdultSize = data["universalMinAdultSize"];
int config_universalMaxSpeed = data["universalMaxSpeed"];
int config_universalMinSpeed = data["universalMinSpeed"];
int config_universalMaxSightReach = data["universalMaxSightReach"];
int config_universalMinSightReach = data["universalMinSightReach"];
int config_geneMutationProbability = data["geneMutationProbability"];
float config_geneMutationFactor = data["geneMutationFactor"];

int config_inputSize = data["inputSize"];
int config_hiddenSize = data["hiddenSize"];
int config_outputSize = data["outputSize"];
int config_brainMutationProbability = data["brainMutationProbability"];
float config_brainMutationFactor = data["brainMutationFactor"];



