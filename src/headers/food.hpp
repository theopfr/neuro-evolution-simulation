#ifndef FOOD_HPP
#define FOOD_HPP

#include <piksel/baseapp.hpp>
#include <iostream>
#include "headers/vector.hpp"
#include "headers/foodType.hpp"
#include "headers/utils.hpp"


class Food {
public:
    Vector position;
    uint size = 6;
    uint currentLifeTime = 0;
    bool disintegrated = false;

    // food type 0: plant, food type 1: meat
    FoodType foodType;
    Food(uint startX, uint startY, FoodType foodType);
    void draw(piksel::Graphics& g);
    void update();
    void rot();
};

#endif