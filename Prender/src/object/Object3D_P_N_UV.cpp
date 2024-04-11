#include "Object3D_P_N_UV.h"
#include <fstream>
#include <sstream>
#include <map>
#include "../../main.h"


//DEPRECATED
Object3D_P_N_UV::Object3D_P_N_UV(std::vector<vec3> points, std::vector<vec3> normals, std::vector<vec2> uvs, std::vector<int> indices, Texture* texture) : nb_vertex(), lightCasterID() {
	vao->setPoints(points, uvs, indices);
#ifdef IMGUI
	ImGuiManager::addObject(ImGuiObjectType::OBJECT_OBJECT3D_DEFAULT_P_N_UV, this);
#endif
#ifdef DEBUG
	debug::NB_INSTANCES++;
#endif
}


Object3D_P_N_UV::Object3D_P_N_UV(const char* file, Material_AR* material) : material(material), nb_vertex(), lightCasterID(), indices(), vertices() {
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
	//std::vector<vertex_P_N_UV> vertex = std::vector<vertex_P_N_UV>();

	std::map<std::tuple<int, int, int>, int> vertexID = std::map<std::tuple<int, int, int>, int>();
	int maxID = 0;

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

			if (vertexID.find(v1) == vertexID.end()) {
				//not in map
				vertexID[v1] = maxID;
				maxID++;
				vertices.push_back(vertex_P_N_UV(points[p1 - 1], normals[n1 - 1], uvs[uv1 - 1]));
			}
			indices.push_back(vertexID[v1]);

			if (vertexID.find(v2) == vertexID.end()) {
				//not in map
				vertexID[v2] = maxID;
				maxID++;
				vertices.push_back(vertex_P_N_UV(points[p2 - 1], normals[n2 - 1], uvs[uv2 - 1]));
			}
			indices.push_back(vertexID[v2]);

			if (vertexID.find(v3) == vertexID.end()) {
				//not in map
				vertexID[v3] = maxID;
				maxID++;
				vertices.push_back(vertex_P_N_UV(points[p3 - 1], normals[n3 - 1], uvs[uv3 - 1]));
			}
			indices.push_back(vertexID[v3]);
		}
	}

	{//compute TBN matrixes
		for (size_t i = 0; i < indices.size(); i += 3) {
			// triangle from :
			//std::cout << "3*i : " << 3*i << " < " << indices.size() << "-3" << std::endl;
			//std::cout << "i1 : " << indices[3 * i + 0] << ", i2 : " << indices[3 * i + 1] << ", i3 : " << indices[3 * i + 2] << " < " << vertices.size() << std::endl;
			vertex_P_N_UV p1 = vertices[indices[i + 0]];
			vertex_P_N_UV p2 = vertices[indices[i + 1]];
			vertex_P_N_UV p3 = vertices[indices[i + 2]];

			vec2 uv1 = p2.uv - p1.uv;
			vec2 uv2 = p3.uv - p1.uv;
			vec3 e1 = p2.point - p1.point;
			vec3 e2 = p3.point - p1.point;

			//uv area to weight the TBN matrix more for big triangle
			float area = length(vec3(uv1)^vec3(uv2))/2;

			//float det = uv1.x * uv2.y - uv1.y * uv2.x;

			/*
			mat2 m = inverse(mat2(uv1.x, uv1.y, uv2.x, uv2.y));

			//handmade matrix product
			//|Tx Ty Tz| = |m0 m1| * |e1x e1y e1z|
			//|Bx By Bz|   |m2 m3|   |e2x e2y e2z|
			vec3 tangeant = vec3(m.c[0] * e1.x + m.c[1] * e2.x, m.c[0] * e1.y + m.c[1] * e2.y, m.c[0] * e1.y + m.c[1] * e2.y);
			vec3 bitangeant = vec3(m.c[2] * e1.x + m.c[3] * e2.x, m.c[2] * e1.y + m.c[3] * e2.y, m.c[2] * e1.y + m.c[3] * e2.y);*/


			//|Tx Ty Tz| = |m0 m1 0| * |e1x e1y e1z|
			//|Bx By Bz|   |m2 m3 0|   |e2x e2y e2z|
			//|0  0  1 |   |0  0  1|   |0   0   1  |

			mat3 m = inverse(mat3(uv1.x, uv1.y, 0, uv2.x, uv2.y, 0, 0, 0, 1));
			mat3 e1e2 = mat3(e1.x, e1.y, e1.z, e2.x, e2.y, e2.z, 0, 0, 1);
			mat3 tb = m * e1e2;
			vec3 tangeant = vec3(tb.c[0], tb.c[1], tb.c[2]);
			vec3 bitangeant = vec3(tb.c[3], tb.c[4], tb.c[5]);


			float a1 = acosf(dot(normalize(p2.point - p1.point), normalize(p3.point - p1.point)));
			float a2 = acosf(dot(normalize(p3.point - p2.point), normalize(p1.point - p2.point)));
			float a3 = acosf(dot(normalize(p1.point - p3.point), normalize(p2.point - p3.point)));

			//p1.tangeant += normalize(tangeant) * area * a1;
			//p2.tangeant += normalize(tangeant) * area * a2;
			//p3.tangeant += normalize(tangeant) * area * a3;

			vertices[indices[i + 0]].tangeant += normalize(tangeant) * area * a1;
			vertices[indices[i + 1]].tangeant += normalize(tangeant) * area * a2;
			vertices[indices[i + 2]].tangeant += normalize(tangeant) * area * a3;
		}
	}
	for (size_t i = 0; i < vertices.size(); i++) {
		//Gram shmidts process
		vertices[i].normal = normalize(vertices[i].normal);//just in case
		//vec3 bi = (vertices[i].normal^vertices[i].tangeant);
		//vec3 ta = (vertices[i].normal ^ vertices[i].tangeant) ^ vertices[i].normal;

		vertices[i].tangeant = normalize((vertices[i].normal ^ vertices[i].tangeant) ^ vertices[i].normal);
		//vertices[i].tangeant = normalize(vertices[i].tangeant);
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
	delete material;
#ifdef DEBUG
	debug::NB_INSTANCES--;
#endif
}

void Object3D_P_N_UV::setup(Shader* shader, const mat4& p, const mat4& v) {
	switch (shader->getShaderType())
	{
	case ShaderType::DEFAULT_P_N_UV:
		vao->bind();
		
		material->setup(shader);
		
		shader->setUniform("lightCasterID", lightCasterID, 5);
		shader->setUniform("mvp", p * v * world * object);
		shader->setUniform("mv", v * world * object);
		shader->setUniform("mv_n", normalTransformation(v * world * object));
		break;
	case ShaderType::DEBUG_TBN:
		vao->bind();

		shader->setUniform("mv", v * world * object);
		shader->setUniform("p", p);
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

	
	

	/*glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(0, -125);
	glDrawElements(GL_TRIANGLES, nb_vertex, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonOffset(0, 0);*/
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
	imGuiObject3D_P_N_UVAttr* attr = new imGuiObject3D_P_N_UVAttr(nb_vertex, tmp, Object3D::getAttribute(), static_cast<void*>(material));
	return attr;
}

void Object3D_P_N_UV::updateAttribute(void* attr) const {

	std::vector<int> tmp = std::vector<int>(maxLight, -1);
	for (int i = 0; i < maxLight; i++) {
		tmp[i] = lightCasterID[i];
	}
		
	static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->nb_vertex = nb_vertex;
	static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->lightCasterID = tmp;
	Object3D::updateAttribute(static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->parentAttr);

}

void Object3D_P_N_UV::setAttribute(void* attr) {
	for (int i = 0; i < maxLight; i++)
		lightCasterID[i] = static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->lightCasterID[i];
	Object3D::setAttribute(static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->parentAttr);
	
}

void Object3D_P_N_UV::imGuiPrintAttribute(void* attr) const {
	ImGui::Text("%d : nb vertex", static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->nb_vertex);

	Object3D::imGuiPrintAttribute(static_cast<imGuiObject3D_P_N_UVAttr*>(attr)->parentAttr);
	if (ImGui::TreeNode("Material")) {
		material->imGuiPrintAttribute(ImGuiManager::getAttr(material));
		ImGui::TreePop();
	}
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