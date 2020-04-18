#pragma once

#include "ofMain.h"
#include "petal.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

      private:
	  	bool paused;
        int size;
		int sizeInc;
		vector<Petal> petals;
		void generate_flower_of_life(int rounds, int center_x, int center_y, int radius, int angle);
};
