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