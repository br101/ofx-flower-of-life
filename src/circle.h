#pragma once

#include "glm/glm.hpp"
#include <vector>

class Circle
{
public:
    Circle(float x, float y, float rad) : x(x), y(y), r(rad) {}
    Circle(glm::vec2 c, float r) : x(c[0]), y(c[1]), r(r) {}

    void draw();
    void drawLine();

    std::vector<glm::vec2> intersect(Circle& c2);

    glm::vec2 getCenter() {
        return glm::vec2(x, y);
    }

//private:
    float x;
    float y;
    float r;

    bool calculate_angle(glm::vec2 location_1, float radius_1, glm::vec2 location_2, float radius_2, double& theta, double& a);
};
