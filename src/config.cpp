#include "headers/config.hpp"

std::ifstream f("../src/config.json");
nlohmann::json data = nlohmann::json::parse(f);

int config_windowWidth = data["windowWidth"];
int config_windowHeight = data["windowHeight"];

int config_initialGenerationSize = data["initialGenerationSize"];
int config_initialFoodAmount = data["initialFoodAmount"];
int config_universalMaxAdultSize = data["universalMaxAdultSize"];
int config_universalMinAdultSize = data["universalMinAdultSize"];
int config_universalMaxSightAngle = data["universalMaxSightAngle"];
int config_universalMinSightAngle = data["universalMinSightAngle"];
int config_universalMaxSightReach = data["universalMaxSightReach"];
int config_universalMinSightReach = data["universalMinSightReach"];
int config_geneMutationProbability = data["geneMutationProbability"];
float config_geneMutationFactor = data["geneMutationFactor"];

int config_inputSize = data["inputSize"];
int config_hiddenSize = data["hiddenSize"];
int config_outputSize = data["outputSize"];
int config_brainMutationProbability = data["brainMutationProbability"];
float config_brainMutationFactor = data["brainMutationFactor"];

float config_wanderingStrength = data["wanderingStrength"];
float config_minSpeed = data["minSpeed"];
float config_maxSpeed = data["maxSpeed"];
float config_currentSize = data["currentSize"];
float config_energyLoss = data["energyLoss"];
float config_wallDamage = data["wallDamage"];
float config_maxEnergyGainByFood = data["maxEnergyGainByFood"];
float config_horninessRegeneration = data["horninessRegeneration"];