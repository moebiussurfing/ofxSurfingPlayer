#pragma once

#include "ofMain.h"

#include "ofxSurfingPlayer.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void draw();
		void keyPressed(int key);

		SurfingPlayer surfingPlayer;
		ofEventListener listener_Beat;

		ofxPanel gui;

		ofxSurfing_ImGui_Manager guiManager;

		float a = 1;
};
