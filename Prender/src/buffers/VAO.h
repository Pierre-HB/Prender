#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "VBO.h"
#include "EOB.h"
#include "../algebra/Algebra.h"
#include "../../main.h"

//! a mesh vertex made from one point, one normal and on iv coordinate to setup the vbo
struct vertex_P_N_UV
{
	vec3 point;
	vec3 normal;
	vec2 uv;

	//! construction of a vertex (Point Normal UV) from a point, a normal and a uv
	vertex_P_N_UV(const vec3& point, const vec3& normal, const vec2& uv) : point(point), normal(normal), uv(uv){}
};

//! Vertex Array Object
class VAO
{
private:

	GLuint vao;
	VBO* vbo;
	EOB* eob;

public:

	//! constructor
	VAO();

	~VAO();

	//! initialize the vbo with points. DEPRECATED
	void setPoints(const std::vector<float>& points) const;

	//! initialize the vbo with points. use an element buffer. DEPRECATED
	void setPoints(const std::vector<float>& points, const std::vector<int>& indices) const;

	//! vbo initialise with a set of point and uv coordinate using an element buffer. DEPRECATED
	void setPoints(const std::vector<float>& points, const std::vector<float>& uvs, const std::vector<int>& indices) const;

	//! vbo initialise with a set of point and uv coordinate using an element buffer. DEPRECATED
	void setPoints(const std::vector<vec3>& points, const std::vector<vec2>& uvs, const std::vector<int>& indices) const;

	//! Propoer way to initialize the VBO. Use an array of vertex_P_N_UV. Use an element buffer
	void setPoints(const std::vector<vertex_P_N_UV>& vertices, const std::vector<int>& indices) const;

	//! bind
	void bind() const;

	//! unbind
	void unbind() const;
};