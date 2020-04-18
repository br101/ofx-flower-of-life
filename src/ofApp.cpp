#include "ofApp.h"
#include "circle.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofEnableAntiAliasing();
    ofEnableAlphaBlending();
    ofSetCircleResolution(50);
    size = 500;
}

//--------------------------------------------------------------
void ofApp::update(){

    size--;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255);
    
    Circle c1(250, 100, size);
    Circle c2(250, 250, size);
    
    c1.draw();
    c2.draw();
    vector<glm::vec2> ints = c1.intersect(c2);
    ofFill();
    ofSetColor(128);
    for (auto i : ints) {
		ofDrawCircle(i, 3);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
