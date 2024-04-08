#include "Light_Constant_Point.h"

Light_Constant_Point::Light_Constant_Point(const vec3& lightColor, const vec3& position) : Light(lightColor, 0, position, 0, vec3(), 0) {

#ifdef IMGUI
	ImGuiManager::addObject(ImGuiObjectType::LIGHT_CONSTANT_POINT, this);
#endif
#ifdef DEBUG
	debug::NB_INSTANCES++;
#endif
}

Light_Constant_Point::~Light_Constant_Point() {
#ifdef IMGUI
	ImGuiManager::removeObject(ImGuiObjectType::LIGHT_CONSTANT_POINT, this);
#endif
#ifdef DEBUG
	debug::NB_INSTANCES--;
#endif
}

#ifdef IMGUI
void* Light_Constant_Point::getAttribute() const {
	imGuiLightConstantPointAttr* attr = new imGuiLightConstantPointAttr(lightColor, position);
	return attr;
}

void Light_Constant_Point::updateAttribute(void* attr) const {
	static_cast<imGuiLightConstantPointAttr*>(attr)->color = lightColor/ static_cast<imGuiLightConstantPointAttr*>(attr)->intensity;
	static_cast<imGuiLightConstantPointAttr*>(attr)->position = position;
}

void Light_Constant_Point::setAttribute(void* attr) {
	lightColor = static_cast<imGuiLightConstantPointAttr*>(attr)->color* static_cast<imGuiLightConstantPointAttr*>(attr)->intensity;
	position = static_cast<imGuiLightConstantPointAttr*>(attr)->position;
}

void Light_Constant_Point::imGuiPrintAttribute(void* attr) const {
	ImGui::ColorEdit3("Light color", &static_cast<imGuiLightConstantPointAttr*>(attr)->color.x);
	
	ImGui::DragFloat3("Light position", &static_cast<imGuiLightConstantPointAttr*>(attr)->position.x);

	ImGui::DragFloat("Intensity", &static_cast<imGuiLightConstantPointAttr*>(attr)->intensity, 1, 1e-3f, 1000, NULL, ImGuiSliderFlags_Logarithmic);
}

void Light_Constant_Point::deleteAttribute(void* attr) const {
	delete static_cast<imGuiLightConstantPointAttr*>(attr);
}
#endif