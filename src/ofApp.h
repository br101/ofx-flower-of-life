#pragma once

#include "FlowerOfLife.h"
#include "ofMain.h"
#include "ofxBeat.h"
#include "ofxFFmpegRecorder.h"
#include "ofxGui.h"

#define HIST_SIZE 200

class ofApp : public ofBaseApp
{

public:
	ofApp() {}
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

	void radiusChanged(int& r);
	void roundsChanged(int& r);
	void angleChanged(float& a);

private:
	FlowerOfLife flower;
	bool paused;
	bool saveSvg;
	int size;
	int sizeInc;
	float smoothedVol;

	ofParameter<int> radius;
	ofParameter<int> rounds;
	ofParameter<float> angle;
	ofParameter<bool> showMeta;
	ofParameter<bool> showEgg;
	ofParameter<ofColor> colFill;
	ofParameter<ofColor> colEgg;
	ofParameter<ofColor> colOut;
	ofxPanel gui;
	bool hideGui;

	ofSoundStream soundStream;

	ofFbo mCapFbo;
	ofPixels mPix;
	ofxFFmpegRecorder m_Recorder;

	ofxBeat beat;
	vector<float> histL;
	vector<float> histM;
	vector<float> histH;
};
