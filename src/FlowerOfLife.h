#pragma once

#include "circle.h"
#include <vector>
#include <algorithm>

class Petal : public Circle
{
public:
    Petal(int x, int y, int r, int num, int round)
        : Circle(x, y, r), num(num), round(round) {}
    Petal(glm::vec2 c, int r, int num, int round)
        : Circle(c, r), num(num), round(round) {}

    int num;
    int round;

    bool isPartOfEgg() const {
        return round < 2 || (round == 2 && num % 2 != 0);
    }

    bool isPartOfFruit() const {
        return (round == 0
                || (round == 2 && num % 2 == 0)
                || (round == 4 && num % 4 == 0));
    }
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

    void generateEgg() {
        generate(2);
        petals.erase(std::remove_if(petals.begin(), petals.end(),
                        [](const Petal& p) {
                            return !p.isPartOfEgg();
                        }),
                    petals.end());
    }

    void generateFruit() {
        generate(4);
        petals.erase(std::remove_if(petals.begin(), petals.end(),
                        [](const Petal& p) {
                            return !p.isPartOfFruit();
                        }),
                    petals.end());
    }

    std::vector<glm::vec2> getMetatronsCube() {
        std::vector<glm::vec2> points;
        for (Petal& p : petals) {
            if (p.isPartOfFruit()) {
                points.push_back(p.getCenter());
            }
        }
        return points;
    }

    int center_x;
    int center_y;
    int radius;
    int angle;
    std::vector<Petal> petals;
};
