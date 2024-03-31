#pragma once

#include "../algebra/Algebra.h"
#include "../buffers/VAO.h"

class Object3D
{
protected:

	mat4 object;
	mat4 world;
	VAO* vao;

public:

	Object3D() : object(mat4::identity()), world(mat4::identity()) {
		vao = new VAO();
	}

	~Object3D() {

	}

	void moove(const mat4& transformation);

	virtual void draw();

};