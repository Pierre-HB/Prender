#include "Object3D_P_N_UV.h"
#include <fstream>
#include <sstream>
#include <map>
#include "../../main.h"


Object3D_P_N_UV::Object3D_P_N_UV(std::vector<vec3> points, std::vector<vec3> normals, std::vector<vec2> uvs, std::vector<int> indices, Texture* texture) : texture(texture), nb_vertex(), lightCasterID() {
	vao->setPoints(points, uvs, indices);
#ifdef IMGUI
	ImGuiManager::addObject(ImGuiObjectType::OBJECT_OBJECT3D_DEFAULT_P_N_UV, this);
#endif
#ifdef DEBUG
	debug::NB_INSTANCES++;
#endif
}

Object3D_P_N_UV::Object3D_P_N_UV(const char* file, Texture* texture) : texture(texture), nb_vertex(), lightCasterID() {
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
#ifdef CONSOLE
		std::cout << "[ERROR] shader could not read file '" << file << "'" << std::endl;
#endif
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
	nb_vertex = static_cast<int>(indices.size());
#ifdef IMGUI
	ImGuiManager::addObject(ImGuiObjectType::OBJECT_OBJECT3D_DEFAULT_P_N_UV, this);
#endif
#ifdef DEBUG
	debug::NB_INSTANCES++;
#endif
}

Object3D_P_N_UV::~Object3D_P_N_UV() {
#ifdef IMGUI
	ImGuiManager::removeObject(ImGuiObjectType::OBJECT_OBJECT3D_DEFAULT_P_N_UV, this);
#endif
#ifdef DEBUG
	debug::NB_INSTANCES--;
#endif
}

void Object3D_P_N_UV::setup(Shader* shader, const mat4& p, const mat4& v) {
	switch (shader->getShaderType())
	{
	case ShaderType::DEFAULT_P_N_UV:
		vao->bind();
		texture->bind();
		shader->setUniform("ourTexture", 0);
		
		shader->setUniform("lightCasterID", lightCasterID, 5);
		shader->setUniform("mvp", p * v * world * object);
		shader->setUniform("mv", v * world * object);
		shader->setUniform("mv_n", normalTransformation(v * world * object));
		break;
	default:
#ifdef CONSOLE
		std::cout << "[WARNING] Setup of Object3D_P_N_UV with shader " << shader->getShaderType() << " no setup is performed." << std::endl;
#endif
		break;
	}
}

void Object3D_P_N_UV::draw() {
	glDrawElements(GL_TRIANGLES, nb_vertex, GL_UNSIGNED_INT, 0);
}

void Object3D_P_N_UV::setLightCasterID(const std::vector<lightCaster>& lightCasters) {

	mat4 inv_ow = inverse(world * object);
	float best_intensity[maxLight]{};
	for (int i = 0; i < maxLight; i++) {
		best_intensity[i] = -1.0f;
		lightCasterID[i] = -1;
	}
		

	lightCaster best_lights[maxLight];
	for (size_t i = 0; i < lightCasters.size(); i++) {
		if (lightCasters[i].falloff < 0)
			continue;
		float d = lightCasters[i].falloff == 0.0f ? 1 : length(inv_ow * lightCasters[i].position); //if falloff = 0 d has no importance. avoid computing d for light at infinity
		float intensity = dot(lightCasters[i].lightColor, vec3(0.25, 0.5, 0.25)) / powf(d, lightCasters[i].falloff);
		int intensityIndex = static_cast<int>(i);
		for (int j = 0; j < maxLight; j++) {
			if (best_intensity[j] < intensity) {
				//swap intensity
				float tmp = intensity;
				intensity = best_intensity[j];
				best_intensity[j] = tmp;
				
				//swap the index
				int tmp_i = intensityIndex;
				intensityIndex = lightCasterID[j];
				lightCasterID[j] = tmp_i;
			}
		}
	}
}


#ifdef IMGUI
void* Object3D_P_N_UV::getAttribute() const {
	std::vector<int> tmp = std::vector<int>(maxLight, -1);
	for (int i = 0; i < maxLight; i++)
		tmp[i] = lightCasterID[i];
	imGuiObject3D_P_N_UVAttr* attr = new imGuiObject3D_P_N_UVAttr(texture->getID(), nb_vertex, tmp, Object3D::getAttribute());
	return attr;
}

void Object3D_P_N_UV::updateAttribute(void* attr) const {

	std::vector<int> tmp = std::vector<int>(maxLight, -1);
	for (int i = 0; i < maxLight; i++) {
		tmp[i] = lightCasterID[i];
	}
		
	static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->textureID = texture->getID();
	static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->nb_vertex = nb_vertex;
	static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->lightCasterID = tmp;
	Object3D::updateAttribute(static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->parentAttr);

}

void Object3D_P_N_UV::setAttribute(void* attr) {
	texture->setID(static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->textureID);
	for (int i = 0; i < maxLight; i++)
		lightCasterID[i] = static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->lightCasterID[i];
	Object3D::setAttribute(static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->parentAttr);
	
}

void Object3D_P_N_UV::imGuiPrintAttribute(void* attr) const {
	int tmp = static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->textureID;
	ImGui::InputInt(": TextureID", &tmp);
	static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->textureID = tmp;
	ImGui::Text("%d : nb vertex", static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->nb_vertex);

	Object3D::imGuiPrintAttribute(static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->parentAttr);

	if (ImGui::BeginListBox("listbox 1"))
	{
		for (int n = 0; n < static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->lightCasterID.size(); n++)
		{
			ImGui::InputInt(std::to_string(n).c_str(), &(static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->lightCasterID[n]));
		}
		ImGui::EndListBox();
	}
}

void Object3D_P_N_UV::deleteAttribute(void* attr) const {
	Object3D::deleteAttribute(static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->parentAttr);
	delete static_cast<imGuiObject3D_P_N_UVAttr*>(attr);
}
#endif