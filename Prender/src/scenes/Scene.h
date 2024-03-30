#pragma once

class Scene;

#include "../../Engine.h"

class Scene {
public:

	Scene();

	~Scene();

	virtual void render();

	virtual void update(Engine* engine);
};