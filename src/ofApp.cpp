#include "ofApp.h"
#include "circle.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofEnableAntiAliasing();
    ofEnableAlphaBlending();
    ofSetCircleResolution(50);
    paused = false;
    size = 80;
    sizeInc = 1;
    generate_flower_of_life(4, 500, 400, 80, 0);
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
    for (auto& p : petals) {
        ofFill();
        ofSetColor(ofColor::yellow, 50);
        ofDrawCircle(p.getCenter(), size);
    }
    for (auto& p : petals) {
        ofNoFill();
        ofSetColor(0);
        ofDrawCircle(p.getCenter(), size);
    }
}

void ofApp::generate_flower_of_life(int rounds, int center_x, int center_y, int radius, int angle)
{
    int num = 1;
    int intersectWith = 0;
    petals.push_back(Petal(center_x, center_y, radius, 0, 0));
    for (int r = 1; r < rounds; r++) {
        for (int i = 0; i < r * 6; i++) {
            ofLog(OF_LOG_NOTICE, "round %d num %d i %d", r, num, i);
            if (r == 1 && i == 0) {
                /* second circle (number 1, round 1)
                 * defines the angle of it all */
                int xoff = glm::cos(glm::radians((float)angle)) * radius;
                int yoff = glm::sin(glm::radians((float)angle)) * radius;
                ofLog(OF_LOG_NOTICE, "xoff %d yoff %d ang %d", xoff, yoff, angle);
                petals.push_back(Petal(center_x + xoff, center_y + yoff, radius, num, r));
            } else {
                /* all other circles: intersection of the previous circle
                 * with the circle to intersect with */
                Petal& p = petals[num - 1];
                Petal& p2 = petals[intersectWith];
                ofLog(OF_LOG_NOTICE, "intersect %d (%d/%d) with %d (%d/%d)",
                    num-1, p.x, p.y, intersectWith, p2.x, p2.y);
                auto is = p.intersect(p2);
                for (auto i : is) {
                    ofLog(OF_LOG_NOTICE, "  is %f %f", i[0], i[1]);
                }
                petals.push_back(Petal(is[0], radius, num, r));
            }
            num++;

            /* finding the circle to make the next intersection with */
            if (i == r * 6 - 1) { // last circle of round
                intersectWith = intersectWith + 1;
            } else if (i % r) { // not on 60 degrees angle
                intersectWith = intersectWith + 1;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
     if (key == ' ') paused = !paused;
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
