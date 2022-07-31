#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	//// Optional: Customize some text label
	//surfingPlayer.setNamePanel("Camera");
	//surfingPlayer.setNameSubPanel("My New Camera");

	//-

	//--------------------------------------------------------------
	listener_Beat = surfingPlayer.bPlayerBeatBang.newListener([this](bool& b) {
		ofLogNotice("Beat Bang : ") << (b ? "TRUE" : "FALSE");

		// Do not know which type triggered, just a bang.
		// Do something
		// Flag the Bang! to be processed on update() / next frame.

		bBang = true;
		});

	//-

	// Different Triggers with a dropdown selector
	// we pass a string vector with the names:
	surfingPlayer.setTrigTypesNames({ "Type #0 - Next", "Type #1 - Random", "Type #2 - Params" });

	// We will be notified when the type selector changed:
	//--------------------------------------------------------------
	listener_Type = surfingPlayer.typeTrig.newListener([this](int& i) {
		ofLogNotice("Target type : ") << i;

		// Do something
		a = 1.f; // Bg alpha to max / white (flash)
		});

	//-

	// We will be notified when BPM changed:
	//--------------------------------------------------------------
	listener_Bpm = surfingPlayer.durationBpm.newListener([this](float& f) {
		ofLogNotice("BPM : ") << f;

		// Do something
		a = .25f; // Bg alpha touch / grey (flash)
		});
}
//--------------------------------------------------------------
void ofApp::update() {

	if (bBang)
	{
		bBang = false;

		// Process the Bang here,
		// to which one is selected!

		switch (surfingPlayer.typeTrig)
		{
		case 0: countType0++; break;
		case 1: countType1++; break;
		case 2: countType2++; break;
		}

		//-

		// Draw feedback
		// BG alpha to max / white (flash)
		a = 1.f;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	// Flash Bg 
	{
		// Decrease Bg alpha until 0.
		a -= 0.1;
		a = ofClamp(a, 0, 1);
		ofColor c{ a * 255 };
		ofClear(c);
	}

	//--

	// Display Debug Info
	{
		int x, y, h;
		x = 100;
		y = 50;
		h = 20;
		std::string ss;
		ofColor color{ 128,0,255 };

		// Print Bang!
		ss = bBang ? "*" : " ";
		ofDrawBitmapStringHighlight(ss, x, y);
		y += 1.25 * h;

		// Print the type selected
		ss = "Target: Type #" + ofToString(surfingPlayer.typeTrig);
		ofDrawBitmapStringHighlight(ss, x, y, 0, color);
		y += 1.25f * h;

		// Monitor the targets
		// Each target will receive bangs when its selected.
		// We will colorize the selected type

		ss = "BANGS Counter";
		ofDrawBitmapStringHighlight(ss, x, y, 0, 255);
		y += 1.25f * h;

		ss = "Type #0 : " + ofToString(countType0);
		ofDrawBitmapStringHighlight(ss, x, y, 0, (surfingPlayer.typeTrig == 0 ? color : 255));
		y += h;

		ss = "Type #1 : " + ofToString(countType1);
		ofDrawBitmapStringHighlight(ss, x, y, 0, (surfingPlayer.typeTrig == 1 ? color : 255));
		y += h;

		ss = "Type #2 : " + ofToString(countType2);
		ofDrawBitmapStringHighlight(ss, x, y, 0, (surfingPlayer.typeTrig == 2 ? color : 255));
		y += h;
	}

	//-

	surfingPlayer.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == OF_KEY_RETURN) { surfingPlayer.setPlayToggle(); }
	if (key == 'G') { surfingPlayer.setVisibleToggle(); }
}
