
#include "circle.h"

vector<glm::vec2> Circle::intersect(Circle& c2) {
    vector<glm::vec2> inters(2);
    double theta;
    double a;
    auto p1 = getCenter();
    bool b = calculate_angle(p1, r, c2.getCenter(), c2.r, theta, a);
    if (b) {
        auto i1 = p1 + glm::vec2(r * cos(theta + a), r * sin(theta + a));
		auto i2 = p1 + glm::vec2(r * cos(theta - a), r * sin(theta - a));
 
        inters.push_back(i1);

        if (i1 != i2) {
            inters.push_back(i2);
        }
    }
    return inters;
}

bool Circle::calculate_angle(glm::vec2 location_1, float radius_1, glm::vec2 location_2, float radius_2, double& theta, double& a) { 
	auto distance = glm::distance(location_1, location_2);
	if (distance > radius_1 + radius_2 || distance < radius_1 - radius_2) { 
        return false;
    }
 
	theta = atan2(location_2.y - location_1.y, location_2.x - location_1.x);
	auto cos_a = (pow(distance, 2) + pow(radius_1, 2) - pow(radius_2, 2)) / (2 * distance * radius_1);
	a = acos(cos_a);
	return true;
}

void Circle::draw() {
    ofFill();
    ofSetColor(0, 255, 255, 128);
    ofDrawCircle(x, y, r);
    ofNoFill();
    ofSetColor(0);
    ofDrawCircle(x, y, r);
}
