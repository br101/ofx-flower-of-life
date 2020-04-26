#pragma once

#include "FlowerOfLife.h"
#include "ofMain.h"
#include "ofxBeat.h"
#include "ofxFFmpegRecorder.h"

#define HIST_SIZE 200

class ofApp : public ofBaseApp
{

public:
	ofApp() : flower(512, 512, 80, 30) {}
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void audioIn(ofSoundBuffer& input);

private:
	FlowerOfLife flower;
	bool paused;
	bool saveSvg;
	int normSize;
	int size;
	int sizeInc;
	float smoothedVol;

	ofSoundStream soundStream;

	ofFbo mCapFbo;
	ofPixels mPix;
	ofxFFmpegRecorder m_Recorder;

	ofxBeat beat;
	vector<float> histL;
	vector<float> histM;
	vector<float> histH;
};
