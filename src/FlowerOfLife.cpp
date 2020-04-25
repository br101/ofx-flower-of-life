#include "FlowerOfLife.h"
#include "utils/ofLog.h"

void FlowerOfLife::generate(int rounds)
{
	int num = 1;
	int intersectWith = 0;

	/* first petal (number 0, round 0) is in the center */
	petals.push_back(Petal(center_x, center_y, radius, 0, 0));

	/* create rounds */
	for (int r = 1; r <= rounds; r++) {
		/* create petals in round */
		for (int i = 0; i < r * 6; i++) {
			ofLog(OF_LOG_NOTICE, "round %d num #%d i %d", r, num, i);
			if (r == 1 && i == 0) {
				/* second petal (number 1, round 1) defines the angle */
				float xoff = glm::cos(glm::radians((float)angle)) * radius;
				float yoff = glm::sin(glm::radians((float)angle)) * radius;
				ofLog(OF_LOG_NOTICE, "  xoff %.2f yoff %.2f ang %d", xoff, yoff,
					  angle);
				petals.push_back(
					Petal(center_x + xoff, center_y + yoff, radius, num, r));
			} else {
				/* all other petals: intersection of the previous petal
				 * with the petal to intersect with */
				Petal& p = petals[num - 1];
				Petal& p2 = petals[intersectWith];
				ofLog(OF_LOG_NOTICE,
					  "  intersect #%d (%.2f/%.2f) with #%d (%.2f/%.2f)",
					  num - 1, p.x, p.y, intersectWith, p2.x, p2.y);
				auto is = p.intersect(p2);
				for (auto i : is) {
					ofLog(OF_LOG_NOTICE, "    is %.2f %.2f", i[0], i[1]);
				}
				petals.push_back(Petal(is[1], radius, num, r));
			}
			num++;

			/* change the circle to make the next intersection with on two
			 * conditions: */
			if (i == r * 6 - 1) { // last circle of round
				intersectWith++;
			} else if (i % r) { // not on 60 degrees angle
				intersectWith++;
			}
		}
	}
}
