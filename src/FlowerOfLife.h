#pragma once

#include "circle.h"
#include <vector>

class Petal : public Circle
{
public:
    Petal(int x, int y, int r, int num, int round)
        : Circle(x, y, r), num(num), round(round) {}
    Petal(glm::vec2 c, int r, int num, int round)
        : Circle(c, r), num(num), round(round) {}

    int num;
    int round;
};

class FlowerOfLife
{
public:
    FlowerOfLife() {}

    void generate(int rounds, int center_x, int center_y, int radius, int angle);

    std::vector<Petal> petals;
};
