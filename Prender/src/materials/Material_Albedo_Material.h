#pragma once

#include "material.h"
#include "../../Shader.h"
#include "../utils/Texture.h"
#include "../../main.h"

#ifdef IMGUI
//! data for the Material_AR UI
struct imGuiMaterial_AR_Attr {
	void* albedo;
	void* rougness;

	//! constructor
	imGuiMaterial_AR_Attr(void* albedo, void* rougness) : albedo(albedo), rougness(rougness) {
#ifdef DEBUG
		debug::NB_ATTR++;
#endif
	}

	~imGuiMaterial_AR_Attr() {
#ifdef DEBUG
		debug::NB_ATTR--;
#endif
	}
};

//! a material from an albedo and a roughness texture
class Material_AR : public ImGuiPrintable
#else
//! a material from an albedo and a roughness texture
class Material_AR
#endif
{
private:

	Texture* albedo;
	Texture* roughness;

public:
	//! construction from the albedo and roughness files
	Material_AR(const char* albedo, const char* roughness);

	//! destructor
	~Material_AR();

	//! bind the albedo and roughness texture if needed
	virtual void setup(Shader* shader);

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