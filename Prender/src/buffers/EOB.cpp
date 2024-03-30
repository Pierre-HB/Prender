#include "EOB.h"

EOB::EOB() : eob() {
	glGenBuffers(1, &eob);
}

EOB::~EOB() {
	glDeleteBuffers(1, &eob);
}

void EOB::setData(GLsizeiptr size, const void* indices, GLenum usage) {
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);
}

void EOB::setSubData() {

}

void EOB::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eob);
}

void EOB::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}