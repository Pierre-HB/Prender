#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class EOB
{
private:

	GLuint eob;

public:

	EOB();

	~EOB();

	void setData(GLsizeiptr size, const void* indices, GLenum usage = GL_STATIC_DRAW);

	void setSubData();

	void bind();

	void unbind();
};