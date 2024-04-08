#pragma once

//! Main include of the engine

//! will create a console window and write in it if define
#define CONSOLE
//! will create an ui if define
#define IMGUI

#ifdef CONSOLE
//! print debug messages
#define DEBUG
#endif

#ifdef DEBUG
struct debug
{
	static int NB_MAIN_INSTANCES;
	static int NB_INSTANCES;
	static int NB_ATTR;
	static int NB_OPENGL_PTR;
};
#endif

#define M_PI 3.141592653589793238462643383279502884e+00F

#ifdef CONSOLE
#include <iostream>
#endif

#ifdef IMGUI
#include "src/imgui/imgui.h"
#include "ImGuiManager.h"
#endif


