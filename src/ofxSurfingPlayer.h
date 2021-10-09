#pragma once

#include "ofMain.h"

//----

//#define USE__OFX_SURFING_IM_GUI // -> Optional. Uncomment to use inside an add-on with ImGui add-on.

//----

#ifdef USE__OFX_SURFING_IM_GUI
#include "ofxSurfingImGui.h"
#endif

#include "ofxSurfingHelpers.h" // -> For the settings but can be easely removed.

class SurfingPlayer
{
public:

	SurfingPlayer();
	~SurfingPlayer();

	void setup();
	void draw();
	void update(ofEventArgs & args);
	void keyPressed(ofKeyEventArgs &eventArgs);
	void exit();

#ifdef USE__OFX_SURFING_IM_GUI
	ofxSurfing_ImGui_Manager guiManager;
#endif

	string path_Params_AppSettings = "SurfingPlayer_AppSettings.xml";
	ofParameterGroup params_AppSettings;
	ofParameter<bool> bKeys;

	//--

	// Exposed to Gui
public:
	ofParameter<bool> bGui_Player{ "Player", false };
	ofParameterGroup params_Player{ "Player" };
	ofParameter<bool> bPlayerBeat; // trig beat

private:
	void Changed_Params_Player(ofAbstractParameter &e);

private:
	ofParameter<bool> bMinimize_Player;
	ofParameter<bool> bPlay; // Play
	ofParameter<float> playerDurationBpm; // Bpm
	ofParameter<int> playerDuration;
	ofParameter<float> playerProgress{ "%", 0, 0, 1 };
	ofParameter<int> index{ "index", 0, 0, 8 }; // trig randomize index
	uint32_t playerTimerStarted;
	const float MAX_DURATION_RATIO = 2.0f;

private:
	uint32_t timePlayer;

public:
	//--------------------------------------------------------------
	float getPlayerPct() {
		return playerProgress;
	}

	//--------------------------------------------------------------
	void setPlayToggle() {
		bPlay = !bPlay;
	}
};

