#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "main.h"


//! the clock of the system, will skedule renders and updates
class Engine;

#include "src/scenes/scene.h"


typedef double Time; //for readability

//! a structure for gathering time data
struct TimeData
{
	static const int UPDATE = 0;
	static const int RENDER = 1;
	static const int IDLE = 2;

	int framesLastSecond;
	int ticksLastSecond;
	Time lastStart;
	Time sleepedTime;
	Time sleepedTimeLastSecond;
	Time lastSecond;
	Time renderTime;
	Time renderTimeLastSecond;
	Time updateTime;
	Time updateTimeLastSecond;
	int fps;
	int tps;
	int lastLoopAction;

	TimeData(Time start = 0) {
		framesLastSecond = 0;
		ticksLastSecond = 0;
		sleepedTime = 0;
		sleepedTimeLastSecond = 0;
		lastSecond = start;
		lastStart = start;
		renderTime = 0;
		renderTimeLastSecond = 0;
		updateTime = 0;
		updateTimeLastSecond = 0;
		fps = 0;
		tps = 0;
		lastLoopAction = TimeData::IDLE;
	}
};

//! a structure to know the exact state of the keys
struct keyStates
{
	//! is this action pressed
	bool forward;
	bool backward;
	bool left;
	bool right;

	keyStates() : forward(false), backward(false), left(false), right(false) {
#ifdef DEBUG
		debug::NB_INSTANCES++;
#endif
	}

	~keyStates() {
#ifdef DEBUG
		debug::NB_INSTANCES--;
#endif
	}
};

//! the clock of the system, will skedule renders and updates
class Engine
{
private:

	GLFWwindow* window;
	std::vector<Scene*> scenes;
	int activeScene;

	double fps; //Frame Per Second
	double tps; //Ticks Per Second

	Time start;
	Time nextRender;//time at wich start the next render
	Time nextUpdate;//time at wich start the next update

	double maxMissedFrames; //max frames to miss after what frame will be discarded. see Engine::run for more explanation
	double maxMissedTicks; //max ticks to miss after what ticks will be discarded. see Engine::run for more explanation

	TimeData timeData;//mesured time data

	keyStates* previousKeyState; //previous actions states

#ifdef IMGUI
	ImGuiManager* imGuiManager; //! ui handler
#endif

public:
	keyStates* currentKeyState; // currente action state
	keyStates* toggleKeyState; // if the action is active and was inactive the previous frame

	static keyStates* bufferKeyState; // buffer of states to gather inputs during ticks

	//! initialize the game
	Engine();

	~Engine();

	//! create a window
	GLFWwindow* initWindows(int width, int height);

	//! the update of the game
	void update();

	//! the render of the game
	void render();

	//! start the game loop
	void run();

	//! add a scene in the scene list. This will NOT change the current scene
	void addScene(Scene* scene);

	//! Change the current scene
	void setActiveScene(int sceneID);

	//! swap the buffers input to update them
	void updateInput();

	//! get the FPS of the engin
	double get_fps() const;

	//! get the TPS of the engin
	double get_tps() const;
};