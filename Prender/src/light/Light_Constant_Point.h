#pragma once

#include "Light.h"

#ifdef IMGUI
struct imGuiLightConstantPointAttr {
	vec3 color;
	vec3 position;

	imGuiLightConstantPointAttr(const vec3& color, const vec3& position) : color(color), position(position) {}
};

class Light_Constant_Point : public Light, ImGuiPrintable
#else
class Light_Constant_Point : public Light
#endif
{
public:
	Light_Constant_Point(const vec3& lightColor, const vec3& position);

	~Light_Constant_Point();

#ifdef IMGUI
	virtual void* getAttribute() const;

	virtual void updateAttribute(void* attr) const;

	virtual void setAttribute(void* attr);

	virtual void imGuiPrintAttribute(void* attr) const;
#endif

};