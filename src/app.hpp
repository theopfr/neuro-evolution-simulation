#ifndef APP_HPP
#define APP_HPP

#include <piksel/baseapp.hpp>
#include "being.cpp"
#include "food.cpp"
#include <vector>


class App : public piksel::BaseApp {
public:
    App() : piksel::BaseApp(1100, 900) {}
    void setup();
    void draw(piksel::Graphics& g);

    uint iteration;
    std::vector<Being> beings;
    std::vector<Food> foods;

    float totalLifeTimes = 0.0;
    int totalLifes = 0;

    void spawnInitialGeneration(uint amount) {
        for (uint i=0; i<amount; i++) {
            beings.push_back(Being(randomInt(20, 1080), randomInt(20, 890)));
        }
    }

    void spawnInitialFood(uint amount) {
        for (uint i=0; i<amount; i++) {
            foods.push_back(Food(randomInt(20, 1080), randomInt(20, 890)));
        }
    }
};
#endif /* APP_HPP */


