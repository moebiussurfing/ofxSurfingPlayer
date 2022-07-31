#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	//// Optional: Customize some text label
	//surfingPlayer.setNamePanel("Camera");
	//surfingPlayer.setNameSubPanel("My New Camera");

	//-

	//--------------------------------------------------------------
	listener_Beat = surfingPlayer.bPlayerBeatBang.newListener([this](bool &b) {
		ofLogNotice("Beat Bang : ") << (b ? "TRUE" : "FALSE");

		// Do not know which type triggered, just a bang.
		// Do something
		// Flag the Bang! to be processed on update() / next frame.

		bBang = true;
	});

	//-

	// Different Triggers with a dropdown selector
	// we pass a string vector with the names:
	surfingPlayer.setTrigTypesNames({ "Type #1 - Next", "Type #2 - Random", "Type #3 - Params" });

	// We will be notified when the type selector changed:
	//--------------------------------------------------------------
	listener_Type = surfingPlayer.typeTrig.newListener([this](int &i) {
		ofLogNotice("Target type : ") << i + 1;

		// Do something
		a = 1.f; // Bg alpha to max / white (flash)
	});

	//-

	// We will be notified when Bpm changed:
	//--------------------------------------------------------------
	listener_Bpm = surfingPlayer.durationBpm.newListener([this](float &f) {
		ofLogNotice("Bpm : ") << f;

		// Do something
		a = .25f; // Bg alpha touch / grey (flash)
	});
}

//--------------------------------------------------------------
void ofApp::draw() {

	// Flash Bg 
	a -= 0.1;// Decrease Bg alpha until 0.
	a = ofClamp(a, 0, 1);
	ofColor c{ a * 255 };
	ofClear(c);

	//-

	int x, y, h;
	x = 100;
	y = 50;
	h = 20;
	std::string ss;
	ofColor color{ 128,0,255 };

	// Print Bang!
	ss = bBang ? "o" : "-";
	ofDrawBitmapStringHighlight(ss, x, y);
	y += 1.25 * h;

	// Print the type selected
	ss = "SELECTED: typeTrig #" + ofToString(surfingPlayer.typeTrig + 1);
	ofDrawBitmapStringHighlight(ss, x, y);
	y += 1.25 * h;

	// Monitor the targets
	// Each target will receive bangs when its selected.
	// We will colorize the selected type

	ss = "Type #0 Counter: " + ofToString(countType0);
	ofDrawBitmapStringHighlight(ss, x, y, 0, (surfingPlayer.typeTrig == 0 ? color:255));
	y += h;

	ss = "Type #1 Counter: " + ofToString(countType1);
	ofDrawBitmapStringHighlight(ss, x, y, 0, (surfingPlayer.typeTrig == 1 ? color : 255));
	y += h;

	ss = "Type #2 Counter: " + ofToString(countType2);
	ofDrawBitmapStringHighlight(ss, x, y, 0, (surfingPlayer.typeTrig == 2 ? color : 255));
	y += h;


	//-

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

	//-

	surfingPlayer.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) 
{
	if (key == OF_KEY_RETURN) { surfingPlayer.setPlayToggle(); }
	if (key == 'G') { surfingPlayer.setVisibleToggle(); }
}
