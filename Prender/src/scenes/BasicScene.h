#pragma once

#include "scene.h"
#include "../utils/texture.h"
#include "../../Shader.h"
#include <vector>
#include "../buffers/VAO.h"
#include "../utils/PerspectiveCamera.h"
#include "../object/Object3D_P_N_UV.h"

class BasicScene : public Scene
{
private:
	Texture* texture;
	Texture* texture_smiley;
	Shader* shader_test;
	PerspectiveCamera* camera;
	Object3D_P_N_UV* object;

public:
	BasicScene();

	~BasicScene();

	void render();

	void update(Engine* engine);
};