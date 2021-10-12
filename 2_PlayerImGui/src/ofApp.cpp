#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// Player
	//--------------------------------------------------------------
	listener_Beat = surfingPlayer.bPlayerBeat.newListener([this](bool &b) {
		ofLogNotice("BEAT: ") << (b ? "TRUE" : "FALSE");

		// Do something
		a = 1.f; // Bg alpha to max / white (flash)
	});

	gui.setup(surfingPlayer.params_Player);

	//-

#ifdef USE__OFX_SURFING_IM_GUI
	surfingPlayer.setNamePanel("Camera");
	surfingPlayer.setNameSubPanel("My New Camera");
#endif
}

//--------------------------------------------------------------
void ofApp::draw() {

	// Flash Bg 
	a -= 0.1;// Decrease Bg alpha
	a = ofClamp(a, 0, 1);
	ofColor c{ a * 255 };
	ofClear(c);

#ifndef USE__OFX_SURFING_IM_GUI
	gui.draw();
#endif

#ifdef USE__OFX_SURFING_IM_GUI
	surfingPlayer.draw();
#endif
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == OF_KEY_RETURN) { surfingPlayer.setPlayToggle(); }
}
