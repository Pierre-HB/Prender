#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "VBO.h"
#include "EOB.h"
#include "../algebra/Algebra.h"

struct vertex_P_N_UV
{
	vec3 point;
	vec3 normal;
	vec2 uv;

	vertex_P_N_UV(const vec3& point, const vec3& normal, const vec2& uv) : point(point), normal(normal), uv(uv){}
};


struct drawInfo
{

};

class VAO
{
private:

	GLuint vao;
	VBO* vbo;
	EOB* eob;

public:

	VAO();

	~VAO();

	//buffer made of points
	void setPoints(const std::vector<float>& points) const;

	//buffer made of points using an element buffer
	void setPoints(const std::vector<float>& points, const std::vector<int>& indices) const;

	//buffer made of point and uv coordinate using an element buffer
	void setPoints(const std::vector<float>& points, const std::vector<float>& uvs, const std::vector<int>& indices) const;

	void setPoints(const std::vector<vec3>& points, const std::vector<vec2>& uvs, const std::vector<int>& indices) const;

	void setPoints(const std::vector<vertex_P_N_UV>& vertices, const std::vector<int>& indices) const;

	void bind() const;

	void unbind() const;
};