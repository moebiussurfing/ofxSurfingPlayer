#pragma once

#include "ofMain.h"

#include "ofxSurfingPlayer.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);

		SurfingPlayer surfingPlayer;
		ofEventListener listener_Beat;

		ofxPanel gui;

		float a = 1;

		ofxSurfing_ImGui_Manager guiManager;
};
