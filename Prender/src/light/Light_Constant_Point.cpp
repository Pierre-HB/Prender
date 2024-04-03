#include "Light_Constant_Point.h"

Light_Constant_Point::Light_Constant_Point(const vec3& lightColor, const vec3& position) : Light(lightColor, 0, position, 0, vec3(), 0) {

#ifdef IMGUI
	ImGuiManager::addObject(ImGuiObjectType::LIGHT_CONSTANT_POINT, this);
#endif
}

Light_Constant_Point::~Light_Constant_Point() {
#ifdef IMGUI
	ImGuiManager::removeObject(ImGuiObjectType::LIGHT_CONSTANT_POINT, this);
#endif
}

#ifdef IMGUI
void* Light_Constant_Point::getAttribute() const {
	imGuiLightConstantPointAttr* attr = new imGuiLightConstantPointAttr(lightColor, position);
	return attr;
}

void Light_Constant_Point::updateAttribute(void* attr) const {
	static_cast<imGuiLightConstantPointAttr*>(attr)->color = lightColor;
	static_cast<imGuiLightConstantPointAttr*>(attr)->position = position;
}

void Light_Constant_Point::setAttribute(void* attr) {
	lightColor = static_cast<imGuiLightConstantPointAttr*>(attr)->color;
	position = static_cast<imGuiLightConstantPointAttr*>(attr)->position;
}

void Light_Constant_Point::imGuiPrintAttribute(void* attr) const {
	ImGui::Text("test");
	float intensity = length(static_cast<imGuiLightConstantPointAttr*>(attr)->color);
	static_cast<imGuiLightConstantPointAttr*>(attr)->color = static_cast<imGuiLightConstantPointAttr*>(attr)->color / std::max(1.0f, intensity);
	ImGui::ColorEdit3("Light color", &static_cast<imGuiLightConstantPointAttr*>(attr)->color.x);
	
	ImGui::DragFloat3("Light position", &static_cast<imGuiLightConstantPointAttr*>(attr)->position.x);
	ImGui::DragFloat("Intensity", &intensity);
	static_cast<imGuiLightConstantPointAttr*>(attr)->color *= std::max(1.0f, intensity);

}
#endif