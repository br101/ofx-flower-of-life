#include "ofApp.h"
#include "circle.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);
	ofEnableAntiAliasing();
	ofEnableAlphaBlending();
	ofSetCircleResolution(72);
	ofSetLineWidth(1);
	paused = true;
	saveSvg = false;
	normSize = 80;
	size = normSize;
	sizeInc = 1;
	flower.generate(4);

	ofSoundStreamSettings settings;
	auto devices = soundStream.getMatchingDevices("default");
	if(!devices.empty()){
		settings.setInDevice(devices[0]);
	}

	settings.setInListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 0;
	settings.numInputChannels = 1;
	settings.bufferSize = 256;
	soundStream.setup(settings);

	mCapFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);

	//m_Recorder.setup(true, false, glm::vec2(ofGetWidth(), ofGetHeight()));
	//m_Recorder.setOverWrite(true);
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (paused)
		return;

	size = normSize * smoothedVol * 50;
	// size = normSize * beat.kick();
	/*
		size += sizeInc;
		if (size == 20) {
			sizeInc = 1;
		} else if (size == 100) {
			sizeInc = -1;
		}
	*/
	beat.update(ofGetElapsedTimeMillis());

	histL.push_back(beat.kick());
	histM.push_back(beat.snare());
	histH.push_back(beat.hihat());

	if (histL.size() >= HIST_SIZE) {
		histL.erase(histL.begin(), histL.begin() + 1);
		histM.erase(histM.begin(), histM.begin() + 1);
		histH.erase(histH.begin(), histH.begin() + 1);
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{

	if (saveSvg) {
		ofBeginSaveScreenAsSVG("flower.svg");
	}

	mCapFbo.begin();

	ofBackground(255);

	// flower: fill
	ofFill();
	for (auto& p : flower.petals) {
		if (p.isPartOfFruit()) {
			ofSetColor(ofColor::orange, 60);
		} else {
			ofSetColor(ofColor::yellow, 20);
		}
		ofDrawCircle(p.getCenter(), size);
		// ofSetColor(0);
		// ofDrawBitmapString(to_string(p.num), p.getCenter());
	}

	// flower: outlines
	ofNoFill();
	ofSetColor(128);
	ofSetLineWidth(1);
	for (auto& p : flower.petals) {
		ofDrawCircle(p.getCenter(), size);
	}

#if 0
    // metatrons cube
    ofSetColor(0);
    ofSetLineWidth(3);
    vector<glm::vec2> points = flower.getMetatronsCube();
    for (size_t i = 0; i < points.size(); i++) {
        for (size_t j = i + 1; j < points.size(); j++) {
            ofDrawLine(points[i], points[j]);
        }
    }

    // enclosing circle
    ofDrawCircle(flower.petals[0].getCenter(),
        glm::distance(flower.petals[0].getCenter(), flower.petals.back().getCenter()) 
            /*+ flower.petals[0].r*/);
#endif

	if (saveSvg) {
		ofEndSaveScreenAsSVG();
		saveSvg = false;
	}

#if 0
    ofDrawBitmapString(to_string(smoothedVol), 20, 20);
    ofDrawBitmapString(to_string(beat.kick()), 20, 40);

    ofSetLineWidth(1);
	ofBeginShape();
	for (unsigned int i = 0; i < histL.size(); i++){
		ofVertex(i*2, 100 - histL[i]*100.0f);
	}
	ofEndShape(false);

    ofBeginShape();
	for (unsigned int i = 0; i < histM.size(); i++){
		ofVertex(i*2, 200 - histM[i]*100.0f);
	}
	ofEndShape(false);

    ofBeginShape();
	for (unsigned int i = 0; i < histH.size(); i++){
		ofVertex(i*2, 300 - histH[i]*100.0f);
	}
	ofEndShape(false);
#endif

	mCapFbo.end();
	mCapFbo.draw(0, 0);

	if (m_Recorder.isRecording()) {
		mCapFbo.readToPixels(mPix);
		if (mPix.getWidth() > 0 && mPix.getHeight() > 0) {
			m_Recorder.addFrame(mPix);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == ' ')
		paused = !paused;
	else if (key == 'r') {
		paused = true;
		size = normSize;
	} else if (key == 'S') {
		ofImage img;
		img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		img.save("screenshot.png");
	} else if (key == 's') {
		saveSvg = true;
	} else if (key == 'f') {
		ofToggleFullscreen();
	} else if (key == 'a') {
		if (m_Recorder.isRecording()) {
			m_Recorder.stop();
		} else {
			m_Recorder.setOutputPath(
				ofToDataPath(ofGetTimestampString() + ".avi", true));
			m_Recorder.setVideoCodec("libx264");
			m_Recorder.setBitRate(8000);
			m_Recorder.startCustomRecord();
		}
	}
}

void ofApp::audioIn(ofSoundBuffer& input)
{
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * input.getRMSAmplitude();
	// smoothedVol = curVol;

	beat.audioReceived(input.getBuffer().data(), input.size(),
					   input.getNumChannels());
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
	ofLog(OF_LOG_NOTICE, "resized %d %d", w, h);
	flower.setCenter(w / 2, h / 2);
	normSize = h / 2 / 5;
	flower.setRadius(normSize);
	flower.clear();
	flower.generate(4);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
