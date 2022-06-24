#include "ofxSurfingPlayer.h"

//--------------------------------------------------------------
SurfingPlayer::SurfingPlayer() {
	ofAddListener(ofEvents().update, this, &SurfingPlayer::update);

	//setup();
}

//--------------------------------------------------------------
SurfingPlayer::~SurfingPlayer() {
	ofRemoveListener(ofEvents().update, this, &SurfingPlayer::update);

	ofRemoveListener(params_Player.parameterChangedE(), this, &SurfingPlayer::Changed_Params_Player);

	exit();
}

//--------------------------------------------------------------
void SurfingPlayer::setup() {
	bSetupDone = true;

	//--

	// Tap Tempo Engine
	{
		bpmTapTempo.setup();

		// Callback to link bpm's
		listener_TapBpm = bpmTapTempo.bpm.newListener([this](float&)
			{
				this->Changed_TapBpm();
			});

		// Beat circle
		circleBeat.setAlpha(0.4);
		circleBeat.setSpeed(0.5);
	}

	//// Beat
	//// Circle Layout
	//{
	//	int w = ofGetWidth();
	//	int h = ofGetHeight();
	//	float _cell = 60;
	//	float _pad = 10;
	//	float _size = _cell * 0.5f;
	//	circleBeat.setRadius(_size);
	//	circleBeat.setPosition(glm::vec2(w / 2.f, h - (_size + 10)));
	//}

	//----	

	bPlay.set("Play", false);
	bTap.set("Tap", false);
	durationBpm.set("BPM", 120, 10, 400);
	durationTime.set("ms", 0, 0, 400 * 8);
	//durationTime.set("ms", 0, 0, 5000);
	bPlayerBeatBang.set("Beat", false); // -> We defined beat to call "load next" palette
	bMinimize_Player.set("Minimize", true);

	bNaturizer.set("Naturizer", false);
	naturizerPower.set("PowerDiv", 4, 2, 4);

	// Gui
	params_Player.setName("SurfingPlayer");
	params_Player.add(bPlay);
	params_Player.add(bTap);
	params_Player.add(durationBpm);
	params_Player.add(durationTime);
	params_Player.add(bNaturizer);
	params_Player.add(naturizerPower);
	params_Player.add(playerProgress);
	params_Player.add(bPlayerBeatBang);
	//params_Player.add(playerDurationRatio);

	//--

	// App Settings
	params_AppSettings.setName("SurfingPlayer");
	params_AppSettings.add(bGui);
	params_AppSettings.add(bGui_WidgetBeat);
	params_AppSettings.add(bPlay);
	params_AppSettings.add(bPlayerBeatBang);
	params_AppSettings.add(durationBpm);
	params_AppSettings.add(durationTime);
	params_AppSettings.add(bNaturizer);
	params_AppSettings.add(naturizerPower);

#ifdef USE__OFX_SURFING_IM_GUI
	params_AppSettings.add(bMinimize_Player);
#endif

	// Exclude
	bTap.setSerializable(false);
	playerProgress.setSerializable(false);
	bPlayerBeatBang.setSerializable(false);
	//bPlay.setSerializable(false);

	ofAddListener(params_Player.parameterChangedE(), this, &SurfingPlayer::Changed_Params_Player);

	//--

	// ImGui

#ifdef USE__OFX_IM_GUI_INSTANTIATED 
	guiManager.setup();
#else
#ifdef USE__OFX_SURFING_IM_GUI
	guiManager.setup(ofxImGuiSurfing::IM_GUI_MODE_NOT_INSTANTIATED); // -> windows will be handle externally
#endif
#endif

	//----

	// Startup

	durationBpm = 120;

	if (name_Instance == "-1") name_Instance = "PLAYER";

	ofxSurfingHelpers::loadGroup(params_AppSettings, path_GLOBAL + path_Params_AppSettings);
}

//--------------------------------------------------------------
void SurfingPlayer::update(ofEventArgs& args)
{
	if (!bSetupDone) setup();

	if (bPlay)
	{
		int d;
		if (!bNaturizer) d = durationTime;
		else d = durationTimeNaturalized;

		uint64_t t = ofGetElapsedTimeMillis();
		timePlayer = t - playerTimerStarted;
		playerProgress = ofMap(timePlayer, 0, d, 0, 1, true);

		if (timePlayer >= d)
		{
			ofLogNotice(__FUNCTION__) << "Done : " << timePlayer;

			playerTimerStarted = ofGetElapsedTimeMillis();

			// Beat
			bPlayerBeatBang = true;

			if (bNaturizer)
			{
				// Randomize
				int r = (int)ofRandom(1, naturizerPower + 1);
				//if (r = 3) r = 4;
				//else if (r = 4) r = 8;
				durationTimeNaturalized = durationTime / r;
				//cout << "durationTimeNaturalized: " << durationTimeNaturalized << endl;
			}
		}
	}
	else
	{
		playerProgress = 0;
	}

	//--

	// Player beat

	if (bGui) circleBeat.update();
	bpmTapTempo.update();

	//if (bBeatPlay)
	//{
	//	uint64_t t = ofGetElapsedTimeMillis();
	//	uint64_t e = t - timelastBeat;
	//	ofLogVerbose(__FUNCTION__) << "Play time:" << e;
	//	int dur = bpmTapTempo.getDurationBar();
	//	beatProgress = ofMap(e, 0, dur, 0, 1, true);
	//	if (e >= dur)
	//	{
	//		bBeatBang = true;
	//	}
	//	else {
	//		//bug
	//		//bBeatBang = false;
	//	}
	//}

	//if (bPlayerBeatBang) bPlayerBeatBang = false;

	//--

	// Beat
	// Circle Layout
	{
		static int _w;
		static int _h;
		int w = ofGetWidth();
		int h = ofGetHeight();
		if (w != _w || h != _h)
		{
			float _cell = 60;
			float _pad = 10;
			float _size = _cell * 0.5f;
			float _gap = 60;
			circleBeat.setRadius(_size);
			circleBeat.setPosition(glm::vec2(w / 2.f - _cell / 2.f, h - (_size + _gap)));
		}
	}
}

//--------------------------------------------------------------
void SurfingPlayer::draw() {

	if (!bGui) return;

	//--

	if (bGui_WidgetBeat) circleBeat.draw();

	//--

	draw_ImGui();
}

//--------------------------------------------------------------
void SurfingPlayer::draw_ImGui() {

	// ImGui

	bool bOpen = true;

	//-

#ifdef USE__OFX_IM_GUI_INSTANTIATED 

	guiManager.begin();
	{
		std::string n;
		n = name_Instance;

		//bOpen = (guiManager.beginWindow(n, bGui));

		bOpen = (guiManager.beginWindow(bGui));

		guiManager.AddLabelBig("PLAYER \n\n" + n, false);

#endif

		//--

#ifdef USE__OFX_SURFING_IM_GUI

		if (bOpen)
		{
			float _w1 = ofxImGuiSurfing::getWidgetsWidth(1);
			float _w2 = ofxImGuiSurfing::getWidgetsWidth(2);
			float _h = ofxImGuiSurfing::getWidgetsHeightUnit();
			float _r = bMinimize_Player ? 0.5 : 1.0;

			guiManager.Add(bMinimize_Player, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

			guiManager.AddSpacingSeparated();

			ofxImGuiSurfing::AddBigToggleNamed(bPlay, _w1, 3.0f * _h * _r, "PLAYING", "PLAY", true, getPlayerProgress());

			guiManager.AddSpacingSeparated();

			guiManager.Add(durationTime, OFX_IM_SLIDER);
			guiManager.Add(durationBpm, OFX_IM_SLIDER);

			if (!bMinimize_Player)
			{
				guiManager.AddSpacing();
				if (ImGui::Button("Half", ImVec2(_w2, _h))) { durationBpm /= 2.f; }
				ImGui::SameLine();
				if (ImGui::Button("Double", ImVec2(_w2, _h))) { durationBpm *= 2.f; }
			}

			if (ImGui::Button("Reset", ImVec2(_w1, _h)))
			{
				durationBpm = 120;
			}

			if (!bMinimize_Player)
			{
				guiManager.AddSpacing();
				guiManager.Add(bNaturizer, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
				if (bNaturizer)guiManager.Add(naturizerPower, OFX_IM_SLIDER);
			}

			guiManager.AddSpacingSeparated();

			// Tap
			//guiManager.Add(bpmTapTempo.bpm, OFX_IM_STEPPER);
			if (guiManager.Add(bTap, bMinimize_Player ? OFX_IM_BUTTON_SMALL : OFX_IM_BUTTON_BIG))
			{
				bpmTapTempo.bang();
			}

			// Beat Bang Trig
			if (!bMinimize_Player)
			if (!bPlay || !bMinimize_Player) {
				guiManager.Add(bPlayerBeatBang, bMinimize_Player ? OFX_IM_BUTTON_SMALL : OFX_IM_BUTTON_BIG);
			}

			// Type target selector
			if (typesTrigNames.size() > 0) { // bypass if not setted
				if (!bMinimize_Player) {
					ofxImGuiSurfing::AddCombo(typeTrig, typesTrigNames);
				}
			}

			// Progress
			guiManager.Add(playerProgress, OFX_IM_PROGRESS_BAR_NO_TEXT);

			//--

			// Beat Circle
			if (!bMinimize_Player)
			{
				guiManager.AddSpacingSeparated();
				guiManager.Add(bGui_WidgetBeat, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
			}
		}

#endif

		//--

#ifdef USE__OFX_IM_GUI_INSTANTIATED 

		if (bOpen)
		{
			guiManager.endWindow();
		}

		//-
	}
	guiManager.end();

#endif
}

//--------------------------------------------------------------
void SurfingPlayer::keyPressed(ofKeyEventArgs& eventArgs)
{
	const int key = eventArgs.key;

	// Modifiers
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
void SurfingPlayer::Changed_Params_Player(ofAbstractParameter& e)
{
	string name = e.getName();

	if (name != "%") ofLogNotice() << __FUNCTION__ << " " << name << " : " << e;

	if (0) {}

	// Play
	else if (name == bPlay.getName())
	{
		if (bPlay) {
			playerTimerStarted = ofGetElapsedTimeMillis();
		}
	}

	// Tap
	else if (name == bTap.getName())
	{
		if (bTap) {
			bTap = false;
			bpmTapTempo.bang();
		}
	}

	// Durations
	else if (name == durationTime.getName()) // ms
	{
		durationBpm = (playerDurationRatio * 60000.f) / durationTime;

		ofLogNotice(__FUNCTION__) << " " << durationTime << " : " << durationBpm << "bpm";
	}

	else if (name == durationBpm.getName())
	{
		// 60,000 ms (1 minute) / Tempo (BPM) = Delay Time in ms for quarter-note beats
		durationTime = (playerDurationRatio * 60000.f) / durationBpm;

		if (bpmTapTempo.bpm != durationBpm) bpmTapTempo.bpm = durationBpm;

		ofLogNotice(__FUNCTION__) << " " << durationTime << " : " << durationBpm << "bpm";
	}

	/*
	else if (name == playerDurationRatio.getName())
	{
		durationBpm = (playerDurationRatio * 60000.f) / durationTime;

		// 60,000 ms (1 minute) / Tempo (BPM) = Delay Time in ms for quarter-note beats
		//durationTime = (playerDurationRatio * 60000.f) / durationBpm;

		if (bpmTapTempo.bpm != durationBpm) bpmTapTempo.bpm = durationBpm;
		//ofLogNotice(__FUNCTION__) << " " << durationTime << " : " << durationBpm << "bpm";

		//ofLogNotice(__FUNCTION__) << " Ratio : " << playerDurationRatio << " : " << durationBpm << "bpm";
	}
	*/

	else if (name == bNaturizer.getName())
	{
		if (bNaturizer)
		{
			// Randomize
			int r = (int)ofRandom(1, naturizerPower + 1);
			//if (r = 3) r = 4;
			//else if (r = 4) r = 8;
			durationTimeNaturalized = durationTime / r;
			//cout << "durationTimeNaturalized: " << durationTimeNaturalized << endl;
		}
	}

	//-

	//// A. Index selector
	//else if (name == index.getName())
	//{
	//	ofLogVerbose(__FUNCTION__) << "index: " << index.get();
	//}

	// B. Beat
	else if (name == bPlayerBeatBang.getName() && bPlayerBeatBang.get())
	{
		ofLogVerbose(__FUNCTION__) << "Beat";

		circleBeat.bang();

		bPlayerBeatBang = false;
	}
}

//--------------------------------------------------------------
void SurfingPlayer::Changed_TapBpm() {
	ofLogNotice(__FUNCTION__) << bpmTapTempo.bpm;

	durationBpm = bpmTapTempo.bpm;

	//setBpm(bpmTapTempo.bpm);
}

//--------------------------------------------------------------
void SurfingPlayer::exit() {
	ofLogNotice(__FUNCTION__);

	ofxSurfingHelpers::saveGroup(params_AppSettings, path_GLOBAL + path_Params_AppSettings);
}

