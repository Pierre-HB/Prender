#pragma once

#include "Light.h"

#ifdef IMGUI
//! attributes for the Light Constant Point UI
struct imGuiLightConstantPointAttr {
	vec3 color;
	vec3 position;
	float intensity;

	//! constructor
	imGuiLightConstantPointAttr(const vec3& color, const vec3& position) : color(color), position(position), intensity(1) {}
};

//! a Point light with no falloff
class Light_Constant_Point : public Light, public ImGuiPrintable
#else
//! a Point light with no falloff
class Light_Constant_Point : public Light
#endif
{
public:
	//! constructor
	Light_Constant_Point(const vec3& lightColor, const vec3& position);

	~Light_Constant_Point();

#ifdef IMGUI
	//! \copydoc ImGuiPrintable::getAttribute()
	virtual void* getAttribute() const;

	//! \copydoc ImGuiPrintable::updateAttribute()
	virtual void updateAttribute(void* attr) const;

	//! \copydoc ImGuiPrintable::setAttribute()
	virtual void setAttribute(void* attr);

	//! \copydoc ImGuiPrintable::imGuiPrintAttribute()
	virtual void imGuiPrintAttribute(void* attr) const;
#endif

};