#include "ofApp.h"
#include "circle.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofEnableAntiAliasing();
    ofEnableAlphaBlending();
    ofSetCircleResolution(72);
    ofSetLineWidth(1);
    paused = true;
    saveSvg = false;
    normSize = 80;
    size = 80;
    sizeInc = 1;
    flower.generate(4);

    ofSoundStreamSetup(0, 2); // 2 input channels
}

//--------------------------------------------------------------
void ofApp::update(){
    if (paused) return;

    size = normSize * smoothedVol * 50;
/*
    size += sizeInc;
    if (size == 20) {
        sizeInc = 1;
    } else if (size == 100) {
        sizeInc = -1;
    }
*/
}

//--------------------------------------------------------------
void ofApp::draw(){

    if (saveSvg) {
        ofBeginSaveScreenAsSVG("flower.svg");
    }

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
        //ofSetColor(0);
        //ofDrawBitmapString(to_string(p.num), p.getCenter());
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

    ofDrawBitmapString(to_string(smoothedVol), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ') paused = !paused;
    else if (key == 'r') {
         paused = true;
         size = normSize;
    }
    else if (key == 'S') {
        ofImage img;
        img.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
        img.save("screenshot.png");
    }
    else if (key == 's') {
        saveSvg = true;
    }
    else if (key == 'f') {
        ofToggleFullscreen();
    }
}

void ofApp::audioIn(ofSoundBuffer &input) {
    float curVol = 0.0;
    int numCounted = 0;

    // lets go through each sample and calculate the root mean square
    // which is a rough way to calculate volume
    for (size_t i = 0; i < input.getNumFrames(); i++) {
        float left = input[i * 2] * 0.5;
        float right = input[i * 2 + 1] * 0.5;

        curVol += left * left;
        curVol += right * right;
        numCounted += 2;
    }

    // this is how we get the mean of rms
    curVol /= (float)numCounted;

    // this is how we get the root of rms
    curVol = sqrt(curVol);

    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    //smoothedVol = curVol;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    ofLog(OF_LOG_NOTICE, "resized %d %d", w, h);
    flower.setCenter(w/2, h/2);
    normSize = h/2/5;
    flower.setRadius(normSize);
    flower.clear();
    flower.generate(4);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
