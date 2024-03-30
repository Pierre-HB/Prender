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
	int framesLastSecond;
	int ticksLastSecond;
	Time sleepedTime;
	Time sleepedTimeLastSecond;
	Time lastSecond;
	int fps;
	int tps;

	TimeData(Time start) {
		framesLastSecond = 0;
		ticksLastSecond = 0;
		sleepedTime = 0;
		sleepedTimeLastSecond = 0;
		lastSecond = start;
		fps = 0;
		tps = 0;
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

	keyStates* previousKeyState; //previous actions states

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