#include "ofApp.h"
#include "circle.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofEnableAntiAliasing();
    ofEnableAlphaBlending();
    ofSetCircleResolution(50);
    ofSetLineWidth(2);
    paused = true;
    size = 80;
    sizeInc = 1;
    flower.generateFruit();
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
    ofBackground(255);
    for (auto& p : flower.petals) {
        ofFill();
        ofSetColor(ofColor::yellow, 50);
        ofDrawCircle(p.getCenter(), size);
        ofSetColor(0);
        ofDrawBitmapString(to_string(p.num), p.getCenter());
    }
    for (auto& p : flower.petals) {
        ofNoFill();
        ofSetColor(0);
        ofDrawCircle(p.getCenter(), size);
    }

    // metatrons cube - brute
    for (auto& p : flower.petals) {
        for (auto& p2 : flower.petals) {
            ofDrawLine(p.getCenter(), p2.getCenter());
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
     if (key == ' ') paused = !paused;
     if (key == 'r') {
         paused = true;
         size = 80;
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
