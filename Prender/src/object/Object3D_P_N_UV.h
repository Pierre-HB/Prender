#pragma once

#include "Object3D.h"
#include "../../Shader.h"
#include "../utils/Texture.h"
#include "../light/Light.h"
#include "../materials/Material_Albedo_Material.h"

#ifdef IMGUI
//! data for the Object3D_P_N_UV UI
struct imGuiObject3D_P_N_UVAttr {
	int nb_vertex;
	std::vector<int> lightCasterID;
	void* parentAttr;
	void* material_ptr;

	//! constructor
	imGuiObject3D_P_N_UVAttr(int nb_vertex, const std::vector<int>& lightCasterID, void* parentAttr, void* material_ptr) : nb_vertex(nb_vertex), lightCasterID(lightCasterID), parentAttr(parentAttr), material_ptr(material_ptr) {
#ifdef DEBUG
		debug::NB_ATTR++;
#endif
	}

	~imGuiObject3D_P_N_UVAttr() {
#ifdef DEBUG
		debug::NB_ATTR--;
#endif
	}
};

//! a 3D obeject with normals and uv. Use an albedo texture and WILL use a normal map and a material map
class Object3D_P_N_UV : public Object3D, public ImGuiPrintable
#else
//! a 3D obeject with normals and uv. Use an albedo texture and WILL use a normal map and a material map
class Object3D_P_N_UV : public Object3D
#endif
{
private:

	Material_AR* material;
	int nb_vertex; //! Do I realy need that ???
	static const int maxLight = 5; //! maximum number of light affecting this object. IF CHANGED MUST ALSO BE CHANGED IN THE SHADER CODE

	std::vector<int> indices;
	std::vector<vertex_P_N_UV> vertices;

public:
	int lightCasterID[maxLight]; //id of light affecting this object

	//! Construction from an obj
	Object3D_P_N_UV(const char* file, const char* albedo, const char* roughness);

	//! construction from a set of points, normals, uv and indices
	Object3D_P_N_UV(std::vector<vec3> points, std::vector<vec3> normals, std::vector<vec2> uvs, std::vector<int> indexes, Texture* texture);

	~Object3D_P_N_UV();

	//! give a list of light caster and select the 5 more important to keep track during the fragment shader pass
	void setLightCasterID(const std::vector<lightCaster>& lightCasters);

	//! load all uniform for the drawing of the object
	void setup(Shader* shader, const mat4& vp, const mat4& v);

	//! \copydoc Object3D::draw()
	void draw();

#ifdef IMGUI
	//! \copydoc ImGuiPrintable::getAttribute()
	virtual void* getAttribute() const;

	//! \copydoc ImGuiPrintable::updateAttribute()
	virtual void updateAttribute(void* attr) const;

	//! \copydoc ImGuiPrintable::setAttribute()
	virtual void setAttribute(void* attr);

	//! \copydoc ImGuiPrintable::imGuiPrintAttribute()
	virtual void imGuiPrintAttribute(void* attr) const;

	//! \copydoc ImGuiPrintable::deleteAttribute()
	virtual void deleteAttribute(void* attr) const;
#endif
};