#include "ofApp.h"
#include "circle.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);
	ofEnableAntiAliasing();
	ofEnableAlphaBlending();
	ofSetCircleResolution(72);

	paused = true;
	saveSvg = false;
	hideGui = false;
	sizeInc = 1;

	flowParams.setName("flower of life");
	flowParams.add(radius.set("radius", 80, 10, 300));
	flowParams.add(rounds.set("rounds", 4, 1, 50));
	flowParams.add(angle.set("angle", 0, 1, 90));

	drawParams.setName("drawing");
	drawParams.add(showMeta.set("metatrons cube", false));
	drawParams.add(showEgg.set("show egg", false));
	drawParams.add(colPetalNum.set("color by petal num", true));
	drawParams.add(colRound.set("color by round", false));
	drawParams.add(colFill.set("fill color", ofColor::yellow, ofColor(0,0,0,0), ofColor(255,255,255,255)));
	drawParams.add(colEgg.set("egg color", ofColor::orange, ofColor(0,0,0,0), ofColor(255,255,255,255)));
	drawParams.add(colOut.set("outline color", ofColor::gray, ofColor(0,0,0,0), ofColor(255,255,255,255)));
	drawParams.add(lineWidth.set("line width", 1, 0, 7));
	drawParams.add(opacFill.set("opacity norm", 20, 0, 100));
	drawParams.add(opacEgg.set("opacity egg", 60, 0, 100));

	gui.setup("settings");
	gui.add(flowParams);
	gui.add(drawParams);

	radius.addListener(this, &ofApp::radiusChanged);
	rounds.addListener(this, &ofApp::roundsChanged);
	angle.addListener(this, &ofApp::angleChanged);

	flower.setAngle(angle);
	flower.setCenter(ofGetWidth() / 2, ofGetHeight() / 2);
	flower.setRadius(radius);
	flower.generate(rounds);
	size = radius;

	/* sound input */
	ofSoundStreamSettings settings;
	auto devices = soundStream.getMatchingDevices("default");
	if (!devices.empty()){
		settings.setInDevice(devices[0]);
	}
	settings.setInListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 0;
	settings.numInputChannels = 1;
	settings.bufferSize = 256;
	soundStream.setup(settings);

	rabbit.setApplicationId("ofx rabbit server");
	rabbit.addTransporter(transporter);
	transporter.bind(10000);
	rabbit.expose(flowParams);
	rabbit.expose(drawParams);
	rabbit.update();
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (paused) {
		return;
	}

	size = flower.getRadius() * smoothedVol * 50;

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

	ofBackground(255);

	vector<Petal>& petals = flower.getPetals();

	// flower: fill
	ofFill();
	for (auto& p : petals) {
		if (showEgg && p.isPartOfFruit()) {
			ofSetColor(colEgg, opacEgg);
		} else {
			if (colPetalNum) {
				ofColor colFill2 = ofColor::blue;
				colFill2.setHueAngle(360.0 / flower.getNumPetals() * p.num);
				ofSetColor(colFill2, opacFill);
			} else if (colRound) {
				ofColor colFill2 = ofColor::blue;
				colFill2.setHueAngle(360.0 / flower.getNumRounds() * p.round);
				ofSetColor(colFill2, opacFill);
			} else {
				ofSetColor(colFill, opacFill);
			}
		}
		ofDrawCircle(p.getCenter(), size);
		//ofSetColor(0);
		//ofDrawBitmapString(to_string(p.num), p.getCenter());
	}

	// flower: outlines
	ofNoFill();
	ofSetColor(colOut);
	ofSetLineWidth(lineWidth);
	for (auto& p : petals) {
		ofDrawCircle(p.getCenter(), size);
	}

	if (showMeta) {
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
		ofDrawCircle(
			petals[0].getCenter(),
			glm::distance(petals[0].getCenter(), petals.back().getCenter())
			/*+ flower.petals[0].r*/);
	}

	if (saveSvg) {
		ofEndSaveScreenAsSVG();
		saveSvg = false;
	}

	if (!hideGui) {
		gui.draw();
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
}

void ofApp::keyPressed(int key)
{
	switch (key) {
	case ' ':
		paused = !paused;
		break;
	case 'r':
		paused = true;
		size = flower.getRadius();
		break;
	case 'S': {
		ofImage img;
		img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		img.save("screenshot.png");
		}; break;
	case 's':
		saveSvg = true;
		break;
	case 'f':
		ofToggleFullscreen();
		break;
	case 'h':
		hideGui = !hideGui;
		break;
	case 'q':
		ofExit();
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

void ofApp::windowResized(int w, int h)
{
	ofLog(OF_LOG_NOTICE, "resized %d %d", w, h);
	flower.setCenter(w / 2, h / 2);
	flower.setRadius(h / 2 / 5);
	flower.clear();
	flower.generate(4);
}

void ofApp::radiusChanged(int& r) {
	flower.setRadius(r);
	flower.generate(rounds);
	size = r;
}

void ofApp::roundsChanged(int& r) {
	flower.generate(r);
}

void ofApp::angleChanged(float& a) {
	flower.setAngle(a);
	flower.generate(rounds);
}
