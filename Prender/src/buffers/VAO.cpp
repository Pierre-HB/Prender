#include "VAO.h"
#include <cassert>

#include "../../main.h"


VAO::VAO() : vao(), vbo() {
	
	vbo = new VBO();
	eob = new EOB();
	glGenVertexArrays(1, &vao);
#ifdef DEBUG
	debug::NB_INSTANCES++;
	debug::NB_OPENGL_PTR++;
#endif
}

VAO::~VAO() {
	delete vbo;
	delete eob;
	glDeleteVertexArrays(1, &vao);
#ifdef DEBUG
	debug::NB_INSTANCES--;
	debug::NB_OPENGL_PTR--;
#endif
}

void VAO::setPoints(const std::vector<float>& points) const{
	
	bind();
	vbo->bind();
	vbo->setData(points.size() * sizeof(float), points.data());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	unbind();
}

void VAO::setPoints(const std::vector<float>& points, const std::vector<int>& indices) const {

	bind();
	vbo->bind();
	eob->bind();
	vbo->setData(points.size() * sizeof(float), points.data());
	eob->setData(indices.size() * sizeof(int), indices.data());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	unbind();
}

void VAO::setPoints(const std::vector<float>& points, const std::vector<float>& uvs, const std::vector<int>& indices) const {
	
	assert(2 * points.size() == 3 * uvs.size());
	//data organisation
	size_t n = points.size() / 3;
	std::vector<float> data = std::vector<float>(5 * n);
	for (size_t i = 0; i < n; i++) {
		data[5*i + 0] = points[3 * i + 0];
		data[5*i + 1] = points[3 * i + 1];
		data[5*i + 2] = points[3 * i + 2];
		data[5*i + 3] = uvs[2 * i + 0];
		data[5*i + 4] = uvs[2 * i + 1];
	}

	bind();
	vbo->bind();
	eob->bind();
	vbo->setData(data.size() * sizeof(float), data.data());
	eob->setData(indices.size() * sizeof(int), indices.data());

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	unbind();
}

void VAO::setPoints(const std::vector<vec3>& points, const std::vector<vec2>& uvs, const std::vector<int>& indices) const {

	assert(points.size() == uvs.size());
	//data organisation
	size_t n = points.size();
	std::vector<float> data = std::vector<float>(5 * n);
	for (size_t i = 0; i < n; i++) {
		data[5 * i + 0] = points[i].x;
		data[5 * i + 1] = points[i].y;
		data[5 * i + 2] = points[i].z;
		data[5 * i + 3] = uvs[i].x;
		data[5 * i + 4] = uvs[i].y;
	}

	bind();
	vbo->bind();
	eob->bind();
	vbo->setData(data.size() * sizeof(float), data.data());
	eob->setData(indices.size() * sizeof(int), indices.data());

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	unbind();
}

void VAO::setPoints(const std::vector<vertex_P_N_UV>& vertices, const std::vector<int>& indices) const {
	bind();
	vbo->bind();
	eob->bind();
	vbo->setData(vertices.size() * sizeof(vertex_P_N_UV), vertices.data());
	eob->setData(indices.size() * sizeof(int), indices.data());

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);
	unbind();
}



void VAO::bind() const {
	glBindVertexArray(vao);
}

void VAO::unbind() const {
	glBindVertexArray(0);
}