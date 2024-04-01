#pragma once

#include "main.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Engine;

#include "src/scenes/scene.h"
#include <vector>

typedef double Time; //for readability

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

struct keyStates
{
	//! is this action pressed
	bool forward;
	bool backward;
	bool left;
	bool right;

	keyStates() : forward(false), backward(false), left(false), right(false) {}
};

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
	ImGuiManager* imGuiManager;
#endif

public:
	keyStates* currentKeyState; // currente action state
	keyStates* toggleKeyState; // if the action is active and was inactive the previous frame

	static keyStates* bufferKeyState; // buffer of states to gather inputs during ticks


	Engine();

	~Engine();

	GLFWwindow* initWindows(int width, int height);

	void update();

	void render();

	void run();

	void addScene(Scene* scene);

	void setActiveScene(int sceneID);

	void updateInput();

	double get_fps();

	double get_tps();
};