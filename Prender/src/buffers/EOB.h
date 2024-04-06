#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//! Element Object Buffer
class EOB
{
private:

	GLuint eob;

public:

	EOB();

	~EOB();

	//! initialize the Element Object Buffer with some indices
	void setData(GLsizeiptr size, const void* indices, GLenum usage = GL_STATIC_DRAW);

	//! Not implemented, set a subpart of the element buffer object
	void setSubData();

	//! bind the buffer
	void bind();

	//! unbind the buffer
	void unbind();
};