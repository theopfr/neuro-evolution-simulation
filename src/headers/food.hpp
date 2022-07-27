#ifndef FOOD_HPP
#define FOOD_HPP

#include <piksel/baseapp.hpp>
#include <iostream>
#include <headers/vector.hpp>
#include "headers/foodType.hpp"


class Food {
public:
    Vector position;
    uint size = 6;
    
    // food type 0: plant, food type 1: meat
    FoodType foodType = Plant;
    Food(uint startX, uint startY);
    void draw(piksel::Graphics& g);
};

#endif