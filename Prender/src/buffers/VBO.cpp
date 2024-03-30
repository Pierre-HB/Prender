#include "VBO.h"

VBO::VBO() : vbo() {
	glGenBuffers(1, &vbo);
}

VBO::~VBO() {
	glDeleteBuffers(1, &vbo);
}

void VBO::setData(GLsizeiptr size, const void* data, GLenum usage) const {
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void VBO::setSubData(GLintptr offset, GLsizeiptr size,	const void* data) const {
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VBO::bind() const{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VBO::unbind() const{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}