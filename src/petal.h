#pragma once

#include "circle.h"

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