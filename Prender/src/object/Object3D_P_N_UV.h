#pragma once

#include "Object3D.h"
#include "../../Shader.h"
#include "../utils/Texture.h"

class Object3D_P_N_UV : public Object3D
{
private:
	Texture* texture;
	int nb_vertex;
public:
	Object3D_P_N_UV(const char* file, Texture* texture);

	Object3D_P_N_UV(std::vector<vec3> points, std::vector<vec3> normals, std::vector<vec2> uvs, std::vector<int> indexes, Texture* texture);

	//Object3D_P_N_UV(std::vector<vec3> points, std::vector<vec3> normals, std::vector<vec2> uvs);

	void setup(Shader* shader, const mat4& vp, const mat4& v);

	void draw();
};