#include "VBO.h"
#include "../../main.h"

VBO::VBO() : vbo() {
	glGenBuffers(1, &vbo);
#ifdef DEBUG
	debug::NB_INSTANCES++;
	debug::NB_OPENGL_PTR++;
#endif
}

VBO::~VBO() {

	glDeleteBuffers(1, &vbo);
#ifdef DEBUG
	debug::NB_INSTANCES--;
	debug::NB_OPENGL_PTR--;
#endif
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