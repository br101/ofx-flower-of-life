#include "FlowerOfLife.h"
#include "utils/ofLog.h"

void FlowerOfLife::generate(int rounds, int center_x, int center_y, int radius, int angle)
{
    int num = 1;
    int intersectWith = 0;
    petals.push_back(Petal(center_x, center_y, radius, 0, 0));
    for (int r = 1; r < rounds; r++) {
        for (int i = 0; i < r * 6; i++) {
            ofLog(OF_LOG_NOTICE, "round %d num %d i %d", r, num, i);
            if (r == 1 && i == 0) {
                /* second circle (number 1, round 1)
                 * defines the angle of it all */
                int xoff = glm::cos(glm::radians((float)angle)) * radius;
                int yoff = glm::sin(glm::radians((float)angle)) * radius;
                ofLog(OF_LOG_NOTICE, "xoff %d yoff %d ang %d", xoff, yoff, angle);
                petals.push_back(Petal(center_x + xoff, center_y + yoff, radius, num, r));
            } else {
                /* all other circles: intersection of the previous circle
                 * with the circle to intersect with */
                Petal& p = petals[num - 1];
                Petal& p2 = petals[intersectWith];
                ofLog(OF_LOG_NOTICE, "intersect %d (%d/%d) with %d (%d/%d)",
                    num-1, p.x, p.y, intersectWith, p2.x, p2.y);
                auto is = p.intersect(p2);
                for (auto i : is) {
                    ofLog(OF_LOG_NOTICE, "  is %f %f", i[0], i[1]);
                }
                petals.push_back(Petal(is[0], radius, num, r));
            }
            num++;

            /* finding the circle to make the next intersection with */
            if (i == r * 6 - 1) { // last circle of round
                intersectWith = intersectWith + 1;
            } else if (i % r) { // not on 60 degrees angle
                intersectWith = intersectWith + 1;
            }
        }
    }
}
