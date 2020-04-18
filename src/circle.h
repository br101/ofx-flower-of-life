#pragma once

#include "ofApp.h"

class Circle
{
public:
    Circle(int x, int y, int rad) : x(x), y(y), r(rad) {}

    void draw();

    vector<glm::vec2> intersect(Circle& c2);

    glm::vec2 getCenter() {
        return glm::vec2(x, y);
    }

private:
    int x;
    int y;
    int r;

    bool calculate_angle(glm::vec2 location_1, float radius_1, glm::vec2 location_2, float radius_2, double& theta, double& a);
};
