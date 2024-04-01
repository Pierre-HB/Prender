#pragma once

class Scene;

#include "../../Engine.h"

class Scene {
public:

	Scene();

	virtual ~Scene();

	virtual void render();

	virtual void update(Engine* engine);
};