#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// Player
	//--------------------------------------------------------------
	listener_Beat = surfingPlayer.bPlayerBeatBang.newListener([this](bool &b) {
		ofLogNotice("BEAT: ") << (b ? "TRUE" : "FALSE");

		// Do something
		a = 1.f;
	});

	gui.setup(surfingPlayer.params_Player);
}

//--------------------------------------------------------------
void ofApp::draw() {

	a -= 0.1;
	a = ofClamp(a, 0, 1);
	ofClear(a * 255);

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == OF_KEY_RETURN) { surfingPlayer.setPlayToggle(); }
}
