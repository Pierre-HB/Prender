#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//! vertex buffer object
class VBO
{
private:

	GLuint vbo;

public:

	//! constructor
	VBO();

	~VBO();

	//! Initialize the buffer with dome data (might be NULL and the buffer will only be reserved in GPU memory)
	void setData(GLsizeiptr size, const void* data, GLenum usage=GL_STATIC_DRAW) const;

	//! Rewrite a subpart of the vbo
	void setSubData(GLintptr offset, GLsizeiptr size, const void* data) const;

	//! bind
	void bind() const;

	//! unbind
	void unbind() const;
};