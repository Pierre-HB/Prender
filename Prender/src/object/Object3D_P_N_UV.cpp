#include "Object3D_P_N_UV.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

Object3D_P_N_UV::Object3D_P_N_UV(std::vector<vec3> points, std::vector<vec3> normals, std::vector<vec2> uvs, std::vector<int> indices, Texture* texture) : texture(texture) {
	vao->setPoints(points, uvs, indices);
}

Object3D_P_N_UV::Object3D_P_N_UV(const char* file, Texture* texture) : texture(texture) {
	std::ifstream inputStream;
	std::stringstream stream;
	inputStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		inputStream.open(file);
		stream << inputStream.rdbuf();
		inputStream.close();
	}
	catch (std::ifstream::failure e) {
		std::cout << "[ERROR] shader could not read file '" << file << "'" << std::endl;
	}
	char line[512];
	int max_size = 512;
	
	std::vector<vec3> points = std::vector<vec3>();
	std::vector<vec3> normals = std::vector<vec3>();
	std::vector<vec2> uvs = std::vector<vec2>();
	std::vector<vertex_P_N_UV> vertex = std::vector<vertex_P_N_UV>();

	std::map<std::tuple<int, int, int>, int> vertexID = std::map<std::tuple<int, int, int>, int>();
	int maxID = 0;
	std::vector<int> indices = std::vector<int>();
	std::vector<vertex_P_N_UV> vertices = std::vector<vertex_P_N_UV>();


	while (!stream.eof()) {
		stream.getline(line, max_size);

		if (line[0] == 'v' && line[1] == ' ') {
			vec3 point = vec3();
			sscanf_s(line, "v %f %f %f", &point.x, &point.y, &point.z);
			points.push_back(point);
		}
		if (line[0] == 'v' && line[1] == 'n') {
			vec3 normal = vec3();
			sscanf_s(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
		if (line[0] == 'v' && line[1] == 't') {
			vec2 uv = vec2();
			sscanf_s(line, "vt %f %f", &uv.x, &uv.y);
			uvs.push_back(uv);
		}
		if (line[0] == 'f' && line[1] == ' ') {
			int p1, n1, uv1;
			int p2, n2, uv2;
			int p3, n3, uv3;
			sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &p1, &uv1, &n1, &p2, &uv2, &n2, &p3, &uv3, &n3);
			std::tuple<int, int, int> v1(p1, n1, uv1);
			std::tuple<int, int, int> v2(p2, n2, uv2);
			std::tuple<int, int, int> v3(p3, n3, uv3);

			if (vertexID.find(v1)==vertexID.end()) {
				//not in map
				vertexID[v1] = maxID;
				maxID++;
				vertices.push_back(vertex_P_N_UV(points[p1-1], normals[n1-1], uvs[uv1-1]));
			}
			indices.push_back(vertexID[v1]);

			if (vertexID.find(v2) == vertexID.end()) {
				//not in map
				vertexID[v2] = maxID;
				maxID++;
				vertices.push_back(vertex_P_N_UV(points[p2-1], normals[n2-1], uvs[uv2-1]));
			}
			indices.push_back(vertexID[v2]);

			if (vertexID.find(v3) == vertexID.end()) {
				//not in map
				vertexID[v3] = maxID;
				maxID++;
				vertices.push_back(vertex_P_N_UV(points[p3-1], normals[n3-1], uvs[uv3-1]));
			}
			indices.push_back(vertexID[v3]);
		}
	}
	vao->setPoints(vertices, indices);
	nb_vertex = indices.size();

}

void Object3D_P_N_UV::setup(Shader* shader, const mat4& vp, const mat4& v) {
	vao->bind();
	texture->bind();
	shader->setUniform("ourTexture", 0);
	shader->setUniform("mvp", vp * world);
	shader->setUniform("mv_n", normalTransformation(v*world));
}

void Object3D_P_N_UV::draw() {
	glDrawElements(GL_TRIANGLES, nb_vertex, GL_UNSIGNED_INT, 0);
}