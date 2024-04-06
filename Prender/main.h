#pragma once

#define CONSOLE
#define IMGUI

#ifdef IMGUI
#include "src/imgui/imgui.h"
#include "ImGuiManager.h"
#endif

#ifdef CONSOLE
#include <iostream>
#endif

#define M_PI 3.141592653589793238462643383279502884e+00F