#pragma once

#include "ofMain.h"

/*

TODO:

+ get dropdown target from surfingPresets
+ get ImGui circle widget from ofxBeatClock
	+ drag widget beat circle ?
+ tick sound ?
+ use internal timer from tap widget ?
+ key commands
	+ key toggle enabler

*/


//----

#define USE__OFX_SURFING_IM_GUI // -> Optional. Uncomment to use inside an add-on with ImGui add-on.
#define USE__OFX_IM_GUI_INSTANTIATED // -> Optional. Comment to just draw the widgets only. 
// ImGui context will be instantiated outsite.

//----


/*

	// SNIPPETS

	// Player Code to copy/paste.
	// Look the Example too.

	// Player
#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER
#endif

	//.h
	SurfingPlayer surfingPlayer;
	ofEventListener listener_Beat;
	ofParameter<bool> bRandomPlay{ "Random", false };
	std::vector<std::string> randomTypesPlay = { "Next Index", "Random Index", "Random Params" };
	ofParameter<int> randomTypePlay{ "Type", 0, 0, 2 };

	//--

	// Setup()
	params_AppSettings.add(surfingPlayer.params_AppSettings);
	//--------------------------------------------------------------
	listener_Beat = surfingPlayer.bPlayerBeat.newListener([this](bool &b) {
		ofLogNotice("BEAT: ") << (b ? "TRUE" : "FALSE");

		if (surfingPlayer.bPlay)
		{
			switch (randomTypePlay) // We can decide what to do when Beat calls!
			{
			case 0: doLoadNext(); break;
			case 1: doRandomizeIndex(); break;
			case 2: doRandomizeParams(); break;
			default: break;
			}
		}
	});

	//--

	// Draw()
	// Gui
	ofxImGuiSurfing::AddToggleRoundedButton(surfingPlayer.bGui_Player);
	if (surfingPlayer.bGui_Player)
	{
		ofxImGuiSurfing::AddCombo(randomTypePlay, randomTypesPlay);
	}

	//--

	// Gui
	if (surfingPlayer.bGui_Player)
	{
		ImGuiWindowFlags flagw = ImGuiWindowFlags_None;
		if (guiManager.bAutoResize) flagw += ImGuiWindowFlags_AlwaysAutoResize;

		if (guiManager.beginWindow(surfingPlayer.bGui_Player, flagw))
		{
			surfingPlayer.draw();
		}
		guiManager.endWindow();
	}

	//--

	// KeyPessed()
	if (key == OF_KEY_RETURN) { surfingPlayer.setPlayToggle(); }

*/


#ifdef USE__OFX_SURFING_IM_GUI
#include "ofxSurfingImGui.h"
#endif

#include "ofxSurfingHelpers.h" // -> For the settings but can be easely removed.

#include "BpmTapTempo.h"
#include "CircleBeat.h"

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
	ofParameter<bool> bGui_Player{ "Player", true };
	ofParameterGroup params_Player{ "Player" };
	ofParameter<bool> bPlayerBeat; // Trig beat
	ofParameter<bool> bPlay; // Play

private:
	void Changed_Params_Player(ofAbstractParameter &e);

private:
	ofParameter<bool> bMinimize_Player;
	ofParameter<float> playerDurationBpm; // Bpm
	ofParameter<int> playerDuration;
	ofParameter<float> playerProgress{ "%", 0, 0, 1 };
	ofParameter<int> index{ "index", 0, 0, 8 }; // trig randomize index
	uint32_t playerTimerStarted;

	const float MAX_DURATION_RATIO = 1.0f;
	//const float MAX_DURATION_RATIO = 2.0f;

	BpmTapTempo bpmTapTempo;
	ofEventListener listener_TapBpm;
	void Changed_TapBpm();
	CircleBeat circleBeat;

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

	//-

	// Custom label for ImGui panels
private:
	string namePanel = "-1";
	string nameSubPanel = "-1";
public:
	//--------------------------------------------------------------
	void setNamePanel(string name) {
		namePanel = name;
		bGui_Player.setName("Player " + namePanel);
	}
	//--------------------------------------------------------------
	void setNameSubPanel(string name) {
		nameSubPanel = name;
	}
};

