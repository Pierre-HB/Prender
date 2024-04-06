#pragma once

//! Main include of the engine

//! will create a console window and write in it if define
#define CONSOLE
//! will create an ui if define
#define IMGUI

#ifdef IMGUI
#include "src/imgui/imgui.h"
#include "ImGuiManager.h"
#endif

#ifdef CONSOLE
#include <iostream>
#endif

#define M_PI 3.141592653589793238462643383279502884e+00F