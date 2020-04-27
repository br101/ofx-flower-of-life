#pragma once

#include "FlowerOfLife.h"
#include "ofMain.h"
#include "ofxBeat.h"
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
	void windowResized(int w, int h);
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
	ofParameterGroup flowParams;

	ofParameter<bool> showMeta;
	ofParameter<bool> showEgg;
	ofParameter<bool> colPetalNum;
	ofParameter<bool> colRound;
	ofParameter<ofColor> colFill;
	ofParameter<ofColor> colEgg;
	ofParameter<ofColor> colOut;
	ofParameter<int> lineWidth;
	ofParameter<int> opacFill;
	ofParameter<int> opacEgg;
	ofParameterGroup drawParams;

	ofxPanel gui;
	bool hideGui;

	ofSoundStream soundStream;

	ofxBeat beat;
	vector<float> histL;
	vector<float> histM;
	vector<float> histH;
};
