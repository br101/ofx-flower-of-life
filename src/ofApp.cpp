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
	showAudio = true;
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
	drawParams.add(colFill.set("fill color", ofColor::yellow,
							   ofColor(0, 0, 0, 0),
							   ofColor(255, 255, 255, 255)));
	drawParams.add(colEgg.set("egg color", ofColor::orange, ofColor(0, 0, 0, 0),
							  ofColor(255, 255, 255, 255)));
	drawParams.add(colOut.set("outline color", ofColor::gray,
							  ofColor(0, 0, 0, 0),
							  ofColor(255, 255, 255, 255)));
	drawParams.add(lineWidth.set("line width", 1, 0, 7));
	drawParams.add(opacFill.set("opacity norm", 20, 0, 100));
	drawParams.add(opacEgg.set("opacity egg", 60, 0, 100));
	drawParams.add(smoothFactor.set("smoothing factor", 0.93, 0, 1.0));

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

	/* aubio objects: have to be before sound input setup */
	onset.setup();
	pitch.setup();
	beat.setup();
	bands.setup();
	ofAddListener(onset.gotOnset, this, &ofApp::onsetEvent);
	ofAddListener(beat.gotBeat, this, &ofApp::beatEvent);

	/* sound input */
	ofSoundStreamSettings settings;
	auto devices = soundStream.getMatchingDevices("default");
	if (!devices.empty()) {
		settings.setInDevice(devices[0]);
	}
	settings.setInListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 0;
	settings.numInputChannels = 1;
	settings.bufferSize = 256;
	soundStream.setup(settings);

	// setup the aubio gui objects
	int start = 20;
	int starty = 800;
	beatGui.setup("ofxAubioBeat", "settings.xml", start + 10, starty);
	beatGui.add(bpm.setup("bpm", 0, 0, 250));

	start += 250;
	onsetGui.setup("ofxAubioOnset", "settings.xml", start + 10, starty);
	onsetGui.add(onsetThreshold.setup("threshold", 0, 0, 2));
	onsetGui.add(onsetNovelty.setup("onset novelty", 0, 0, 10000));
	onsetGui.add(onsetThresholdedNovelty.setup("thr. novelty", 0, -1000, 1000));
	onsetThreshold = onset.threshold;

	start += 250;
	pitchGui.setup("ofxAubioPitch", "settings.xml", start + 10, starty);
	pitchGui.add(midiPitch.setup("midi pitch", 0, 0, 128));
	pitchGui.add(pitchConfidence.setup("confidence", 0, 0, 1));

	for (int i = 0; i < 40; i++) {
		bandPlot.addVertex(50 + i * 650 / 40., starty + 240 - 100 * bands.energies[i]);
	}

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
	onset.setThreshold(onsetThreshold);

	// this will recalculate flower
	//if (pitch.pitchConfidence > 0.5) {
	//	angle = pitch.latestPitch;
	//}

	// size = normSize * beat.kick();
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
void ofApp::draw()
{
	if (saveSvg) {
		ofBeginSaveScreenAsSVG("flower.svg");
	}

	ofBackground(0);

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
				//colFill2.setSaturation(255.0 * smoothedVol * 50);
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

	if (showAudio) {
		ofFill();
		if (gotBeat) {
			ofSetColor(ofColor::green);
			ofDrawRectangle(20, 850, 50, 50);
			gotBeat = false;
		}

		if (gotOnset) {
			ofSetColor(ofColor::red);
			ofDrawRectangle(70, 850, 50, 50);
			gotOnset = false;
		}
		onsetNovelty = onset.novelty;
		onsetThresholdedNovelty = onset.thresholdedNovelty;

		pitchConfidence = pitch.pitchConfidence;
		if (pitch.latestPitch)
			midiPitch = pitch.latestPitch;
		bpm = beat.bpm;

		pitchGui.draw();
		beatGui.draw();
		onsetGui.draw();

		ofSetColor(ofColor::orange);
		ofSetLineWidth(3.);
		for (size_t i = 0; i < bandPlot.size(); i++) {
			bandPlot[i].y = 990 - 100 * bands.energies[i];
		}
		bandPlot.draw();
	}
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
	case 'a':
		showAudio = !showAudio;
		break;
	case 'q':
		ofExit();
	}
}

void ofApp::audioIn(ofSoundBuffer& input)
{
	smoothedVol *= smoothFactor;
	smoothedVol += (1.0 - smoothFactor) * input.getRMSAmplitude();
	//smoothedVol = input.getRMSAmplitude();

	onset.audioIn(input.getBuffer().data(), input.size(), input.getNumChannels());
	pitch.audioIn(input.getBuffer().data(), input.size(), input.getNumChannels());
	beat.audioIn(input.getBuffer().data(), input.size(), input.getNumChannels());
	bands.audioIn(input.getBuffer().data(), input.size(), input.getNumChannels());
}

void ofApp::onsetEvent(float & time) {
    //ofLog() << "got onset at " << time << " s";
    gotOnset = true;
}

void ofApp::beatEvent(float & time) {
    //ofLog() << "got beat at " << time << " s";
    gotBeat = true;
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
