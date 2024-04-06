#pragma once

#include "scene.h"
#include "../utils/texture.h"
#include "../../Shader.h"
#include <vector>
#include "../buffers/VAO.h"
#include "../utils/PerspectiveCamera.h"
#include "../object/Object3D_P_N_UV.h"

//! a small scene to showcase features of the engine
class BasicScene : public Scene
{
private:
	Texture* texture;
	Texture* texture_smiley;
	Texture* debug;
	Shader* shader_test;
	PerspectiveCamera* camera;
	Object3D_P_N_UV* object;

	LightManager* lightManager;

public:
	BasicScene();

	~BasicScene();

	//! render of the scene
	void render();

	//! update of the scene
	void update(Engine* engine);
};