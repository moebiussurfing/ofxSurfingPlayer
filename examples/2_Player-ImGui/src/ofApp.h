#pragma once

#include "ofMain.h"

#include "ofxSurfingPlayer.h"

/*
	NOTE:
	To run this example out-of-the-box, 
	you must uncomment the two lines on the top of ofxSurfingPlayer.h :
	#define USE__OFX_SURFING_IM_GUI // -> Optional. Uncomment to use inside an add-on with ImGui add-on.
	#define USE__OFX_IM_GUI_INSTANTIATED // -> Optional. Comment to just draw the widgets only.
	Also you need to include the ofxSurfingImGui add-on into Project Generator.
*/

class ofApp : public ofBaseApp{

	public:
		void setup();
		void draw();
		void keyPressed(int key);

		SurfingPlayer surfingPlayer;

		ofEventListener listener_Beat; // Be notified when Bang happens
		ofEventListener listener_Type; // Be notified when type changes
		ofEventListener listener_Bpm; // Be notified when Bpm changes

		ofxSurfing_ImGui_Manager guiManager;

		bool bBang = false; // True when Bang happens
		
		float a = 1;
		int countType0, countType1, countType2;
};
