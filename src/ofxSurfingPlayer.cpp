#include "ofxSurfingPlayer.h"

//--------------------------------------------------------------
SurfingPlayer::SurfingPlayer() {
	ofAddListener(ofEvents().update, this, &SurfingPlayer::update);

	setup();
}

//--------------------------------------------------------------
SurfingPlayer::~SurfingPlayer() {
	ofRemoveListener(ofEvents().update, this, &SurfingPlayer::update);

	ofRemoveListener(params_Player.parameterChangedE(), this, &SurfingPlayer::Changed_Params_Player);

	exit();
}

//--------------------------------------------------------------
void SurfingPlayer::setup() {

	bPlay.set("Play", false);
	playerDurationBpm.set("Bpm", 120, 10, 400);
	playerDuration.set("ms", 0, 0, 5000);
	bPlayerBeat.set("Beat", false); // -> We defined beat to call "load next" palette
	bMinimize_Player.set("Minimize", false);

	// Gui
	params_Player.setName("SurfingPlayer");
	params_Player.add(bPlay);
	params_Player.add(playerDurationBpm);
	params_Player.add(playerDuration);
	params_Player.add(index);
	params_Player.add(playerProgress);
	params_Player.add(bPlayerBeat);

	// App Settings
	params_AppSettings.setName("SurfingPlayer");
	params_AppSettings.add(bGui_Player);
	params_AppSettings.add(bPlay);
	params_AppSettings.add(bPlayerBeat);
	params_AppSettings.add(playerDurationBpm);
	params_AppSettings.add(playerDuration);
	params_AppSettings.add(index);
	params_AppSettings.add(bMinimize_Player);

	// Exclude
	bPlay.setSerializable(false);
	playerProgress.setSerializable(false);
	index.setSerializable(false);
	bPlayerBeat.setSerializable(false);

	ofAddListener(params_Player.parameterChangedE(), this, &SurfingPlayer::Changed_Params_Player);

	//-
	
	// Gui
#ifdef USE__OFX_SURFING_IM_GUI
	guiManager.setup(ofxImGuiSurfing::IM_GUI_MODE_NOT_INSTANTIATED); // -> windows will be handle externally
	//guiManager.setup(ofxImGuiSurfing::IM_GUI_MODE_INSTANTIATED);
#endif

	//--

	// Startup
	playerDurationBpm = 120;

	ofxSurfingHelpers::loadGroup(params_AppSettings);
}

//--------------------------------------------------------------
void SurfingPlayer::update(ofEventArgs & args) {

	if (bPlay)
	{
		uint64_t t = ofGetElapsedTimeMillis();
		timePlayer = t - playerTimerStarted;
		playerProgress = ofMap(timePlayer, 0, playerDuration, 0, 1, true);

		if (timePlayer >= playerDuration)
		{
			ofLogNotice(__FUNCTION__) << "Done : " << timePlayer;

			playerTimerStarted = ofGetElapsedTimeMillis();

			// Randomize
			index = ofRandom(index.getMin(), index.getMax() + 1);

			// Beat
			bPlayerBeat = true;
		}
	}
	else
	{
		playerProgress = 0;
	}
}

//--------------------------------------------------------------
void SurfingPlayer::draw() {
	if (!bGui_Player) return;

#ifdef USE__OFX_SURFING_IM_GUI
	//ImGuiWindowFlags flagw = ImGuiWindowFlags_None;
	//if (guiManager.bAutoResize) flagw += ImGuiWindowFlags_AlwaysAutoResize;

	////IMGUI_SUGAR__WINDOWS_CONSTRAINTS;
	////IMGUI_SUGAR__WINDOWS_CONSTRAINTS_SMALL;

	//if (guiManager.beginWindow(bGui_Player, flagw))
	{
		guiManager.refreshLayout();
		guiManager.resetUniqueNames();

		float _w1 = ofxImGuiSurfing::getWidgetsWidth(1);
		float _w2 = ofxImGuiSurfing::getWidgetsWidth(2);
		float _h = ofxImGuiSurfing::getWidgetsHeightUnit();

		if (guiManager.Add(bMinimize_Player, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL)) {}

		ofxImGuiSurfing::AddBigToggleNamed(bPlay, _w1, 2 * _h, "PLAYING", "PLAY", true, 1 - getPlayerPct());

		if (!bPlay || !bMinimize_Player) {
			guiManager.Add(bPlayerBeat, OFX_IM_BUTTON_BIG);
		}

		//guiManager.Add(index, OFX_IM_DRAG);//not used

		if (!bMinimize_Player) {
			guiManager.Add(playerDurationBpm, OFX_IM_SLIDER);
			IMGUI_SUGAR__SLIDER_ADD_MOUSE_WHEEL(playerDurationBpm, 5);
			//ofxImGuiSurfing::AddMouseWheel(playerDurationBpm, 5);
		}

		guiManager.Add(playerDuration, OFX_IM_SLIDER);
		ofxImGuiSurfing::AddMouseWheel(playerDurationBpm, 1);

		if (!bMinimize_Player) {
			if (ImGui::Button("Half", ImVec2(_w2, _h))) { playerDurationBpm /= 2.f; }
			ImGui::SameLine();
			if (ImGui::Button("Double", ImVec2(_w2, _h))) { playerDurationBpm *= 2.f; }
			if (ImGui::Button("Reset", ImVec2(_w1, _h))) { playerDurationBpm = 120; }
		}

		if (!bMinimize_Player) guiManager.Add(playerProgress, OFX_IM_PROGRESS_BAR_NO_TEXT);
	}
	//guiManager.endWindow();
#endif
}

//--------------------------------------------------------------
void SurfingPlayer::keyPressed(ofKeyEventArgs &eventArgs)
{
	const int key = eventArgs.key;

	// modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	ofLogNotice(__FUNCTION__) << " : " << key;

	//--

	if (0) {}

	//else if (key == 'G' && !mod_ALT)
	//{
	//}
}

//--------------------------------------------------------------
void SurfingPlayer::Changed_Params_Player(ofAbstractParameter &e)
{
	string name = e.getName();

	if (name != " ") ofLogNotice() << __FUNCTION__ << name << " : " << e;

	if (0) {}

	// Play
	else if (name == bPlay.getName())
	{
		if (bPlay) {
			playerTimerStarted = ofGetElapsedTimeMillis();
		}
	}

	// Durations
	else if (name == playerDuration.getName())
	{
		playerDurationBpm = (MAX_DURATION_RATIO * 60000.f) / playerDuration;

		ofLogNotice(__FUNCTION__) << playerDuration << " : " << playerDurationBpm << "bpm";
	}
	else if (name == playerDurationBpm.getName())
	{
		// 60,000 ms (1 minute) / Tempo (BPM) = Delay Time in ms for quarter-note beats
		playerDuration = (MAX_DURATION_RATIO * 60000.f) / playerDurationBpm;

		ofLogNotice(__FUNCTION__) << playerDuration << " : " << playerDurationBpm << "bpm";
	}

	// A. Index selector
	else if (name == index.getName())
	{
		ofLogVerbose(__FUNCTION__) << "rIndex: " << index.get();
	}

	// B. Beat
	else if (name == bPlayerBeat.getName() && bPlayerBeat)
	{
		bPlayerBeat = false;
		ofLogVerbose(__FUNCTION__) << "Beat";

		//colorClient.nextPreset();
	}
}

//--------------------------------------------------------------
void SurfingPlayer::exit() {

	ofxSurfingHelpers::saveGroup(params_AppSettings);
}
