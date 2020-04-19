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
    FlowerOfLife(int center_x, int center_y, int radius, int angle)
        : center_x(center_x), center_y(center_y), radius(radius), angle(angle) {}

    void generate(int rounds);

    void generateSeed() {
        generate(1);
    }

    void generateFlower() {
        generate(2);
    }

    int center_x;
    int center_y;
    int radius;
    int angle;
    std::vector<Petal> petals;
};
