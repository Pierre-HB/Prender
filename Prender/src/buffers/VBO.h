#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VBO
{
private:

	GLuint vbo;

public:

	VBO();

	~VBO();

	void setData(GLsizeiptr size, const void* data, GLenum usage=GL_STATIC_DRAW) const;

	void setSubData(GLintptr offset, GLsizeiptr size, const void* data) const;

	void bind() const;

	void unbind() const;
};