#pragma once

#include "Object3D.h"
#include "../../Shader.h"
#include "../utils/Texture.h"
#include "../light/Light.h"

#ifdef IMGUI
struct imGuiObject3D_P_N_UVAttr {
	GLuint textureID;
	int nb_vertex;
	std::vector<int> lightCasterID;
	void* parentAttr;

	imGuiObject3D_P_N_UVAttr(GLuint textureID, int nb_vertex, const std::vector<int>& lightCasterID, void* parentAttr) : textureID(textureID), nb_vertex(nb_vertex), lightCasterID(lightCasterID), parentAttr(parentAttr) {}
};

class Object3D_P_N_UV : public Object3D, public ImGuiPrintable
#else
class Object3D_P_N_UV : public Object3D
#endif
{
private:

	Texture* texture;
	int nb_vertex;
	static const int maxLight = 5;

public:
	int lightCasterID[maxLight]; //id of light affecting this object

	Object3D_P_N_UV(const char* file, Texture* texture);

	Object3D_P_N_UV(std::vector<vec3> points, std::vector<vec3> normals, std::vector<vec2> uvs, std::vector<int> indexes, Texture* texture);

	~Object3D_P_N_UV();

	//! give a list of light caster and select the 5 more important to keep track during the fragment shader pass
	void setLightCasterID(const std::vector<lightCaster>& lightCasters);

	void setup(Shader* shader, const mat4& vp, const mat4& v);

	void draw();

#ifdef IMGUI
	virtual void* getAttribute() const;

	virtual void updateAttribute(void* attr) const;

	virtual void setAttribute(void* attr);

	virtual void imGuiPrintAttribute(void* attr) const;
#endif
};