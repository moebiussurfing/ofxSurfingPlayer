#pragma once

#include "ofMain.h"

#include "ofxSurfingPlayer.h"
#include "ofxGui.h"


/*

To run this example out-of-the-box you must go to ofxSurfingPlayer.h and
To comment these two lines to completely disable the ImGui helpers!

//#define USE__OFX_SURFING_IM_GUI 
//#define USE__OFX_IM_GUI_INSTANTIATED 

You should use the minimal add-on verison using the OF Core ofxGui.

*/


class ofApp : public ofBaseApp{

	public:
		void setup();
		void draw();
		void keyPressed(int key);

		SurfingPlayer surfingPlayer;
		ofEventListener listener_Beat;

		ofxPanel gui;

		float a = 1;
};
