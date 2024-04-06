#pragma once

class Scene;

#include "../../Engine.h"

//! virtual class for scene. A scene is the environement to simulate.
class Scene {
public:
	//! empty constructor
	Scene();

	virtual ~Scene();

	//! rendering of the scene
	virtual void render();

	//! updating of the scene
	virtual void update(Engine* engine);
};