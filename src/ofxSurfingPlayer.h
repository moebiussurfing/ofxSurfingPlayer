#pragma once

#include "ofMain.h"

/*

TODO:

+ get ImGui circle widget from ofxBeatClock
+ drag widget beat circle ?
+ use internal timer from tap widget ?
+ key commands. key toggle enabler

*/


//----

// -> Comment these two lines to completely disable the ImGui helpers and to use ofxGui!
// -> Uncomment to use ImGui through ofxSurfingImGui

#define USE__OFX_SURFING_IM_GUI // -> Optional. Uncomment to use inside an add-on with ImGui add-on.
#define USE__OFX_IM_GUI_INSTANTIATED // -> Optional. Comment to just draw the widgets only. ImGui context will be instantiated outsite.

//----


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

private:

#ifdef USE__OFX_SURFING_IM_GUI
	ofxSurfing_ImGui_Manager guiManager;
#endif

	std::string path_Params_AppSettings = "SurfingPlayer_AppSettings.xml";
	ofParameter<bool> bKeys;

public:

	ofParameterGroup params_AppSettings;

	//--

	// Exposed to external Gui

public:

	ofParameterGroup params_Player{ "Player" };

	ofParameter<bool> bGui{ "PLAYER", true };
	ofParameter<bool> bPlayerBeatBang; // Trig beat
	ofParameter<bool> bPlay; // Play
	ofParameter<bool> bTap; // Tap

	//--------------------------------------------------------------
	void setName(string name) {
		bGui.setName(name + " PLAYER");
		//bGui.setName("Player " + name);
		//bGui.setName(bGui.getName() + " " + name);
	}

private:

	void Changed_Params_Player(ofAbstractParameter &e);

public:

	ofParameter<int> targetType{ "type", 0, 0, 0 };

	// Bpm/ms. Beat Duration
	ofParameter<float> durationBpm; // Bpm
	ofParameter<int> durationTime; // ms

	ofParameter<bool> bMinimize_Player;
private:
	ofParameter<float> playerProgress{ "%", 0, 0, 1 };
	//ofParameter<int> index{ "index", 0, 0, 8 }; // trig randomize index

	ofParameter<int> playerDurationRatio{ "Ratio", 1, 1, 4 };//TODO: fix to allow runtime..
	//const float playerDurationRatio = 1.0f;

	uint32_t playerTimerStarted;

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

	std::string namePanel = "-1";
	std::string nameSubPanel = "-1";

public:

	//--------------------------------------------------------------
	void setNamePanel(std::string name) {
		namePanel = name;
		bGui.setName(namePanel);
	}
	//--------------------------------------------------------------
	void setNameSubPanel(std::string name) {
		nameSubPanel = name;
	}

	//-

public:

	//--------------------------------------------------------------
	void setTrigTypesNames(std::vector<std::string> namesTrigs)
	{
		typesTrigNames.clear();
		typesTrigNames = namesTrigs;

		typeTrig.setMin(0);
		typeTrig.setMax(typesTrigNames.size() - 1);
	}

private:

	std::vector<std::string> typesTrigNames;


public:

	ofParameter<int> typeTrig{ "Target", 0, 0, 0 };
};

