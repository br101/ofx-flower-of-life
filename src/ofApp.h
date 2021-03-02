#pragma once

#include "FlowerOfLife.h"
#include "ofMain.h"
#include "ofxAubio.h"
#include "ofxGui.h"
#include "ofxRabbitControl.h"

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
	void onsetEvent(float& time);
	void beatEvent(float& time);

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
	ofParameter<bool> showRotate;
	ofParameter<bool> colPetalNum;
	ofParameter<bool> colRound;
	ofParameter<ofColor> colFill;
	ofParameter<ofColor> colEgg;
	ofParameter<ofColor> colOut;
	ofParameter<int> lineWidth;
	ofParameter<int> opacFill;
	ofParameter<int> opacEgg;
	ofParameter<float> smoothFactor;
	ofParameterGroup drawParams;

	ofxPanel gui;
	bool hideGui;

	ofSoundStream soundStream;

	ofxRabbitControlServer rabbit;
	websocketServerTransporter transporter;

	bool showAudio;
	ofxAubioOnset onset;
	ofxAubioPitch pitch;
	ofxAubioBeat beat;
	ofxAubioMelBands bands;

	ofxPanel pitchGui;
	ofxFloatSlider midiPitch;
	ofxFloatSlider pitchConfidence;

	ofxPanel beatGui;
	bool gotBeat;
	ofxFloatSlider bpm;

	ofxPanel onsetGui;
	bool gotOnset;
	ofxFloatSlider onsetThreshold;
	ofxFloatSlider onsetNovelty;
	ofxFloatSlider onsetThresholdedNovelty;

	ofPolyline bandPlot;
};
