#pragma once

#define CONSOLE
#define IMGUI

#ifdef IMGUI
#include "src/imgui/imgui.h"
#endif

#ifdef CONSOLE
#include <iostream>
#endif