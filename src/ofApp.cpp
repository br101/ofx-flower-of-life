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
    size = 80;
    sizeInc = 1;
    flower.generate(4);
}

//--------------------------------------------------------------
void ofApp::update(){
    if (paused) return;

    size += sizeInc;
    if (size == 20) {
        sizeInc = 1;
    } else if (size == 100) {
        sizeInc = -1;
    }
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

    if (saveSvg) {
        ofEndSaveScreenAsSVG();
        saveSvg = false;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ') paused = !paused;
    else if (key == 'r') {
         paused = true;
         size = 80;
    }
    else if (key == 'S') {
        ofImage img;
        img.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
        img.save("screenshot.png");
    }
    else if (key == 's') {
        saveSvg = true;
    }
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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
