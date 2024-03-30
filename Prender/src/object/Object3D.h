#pragma once

#include "../algebra/Algebra.h"
#include "../buffers/VAO.h"

class Object3D
{
protected:

	mat4 world;
	VAO* vao;

public:

	Object3D() : world(mat4::identity()) {
		vao = new VAO();
	}

	~Object3D() {

	}

	void moove(const mat4& transformation);

	virtual void draw();

};