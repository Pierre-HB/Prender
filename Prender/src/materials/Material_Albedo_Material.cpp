#include "Material_Albedo_Material.h"
#include "../../main.h"

//we set the albedo texture on the texture unit 0
//we set the roughness texture on the texture unit 1
Material_AR::Material_AR(const char* albedo, const char* normal_map, const char* roughness) : albedo(new Texture(albedo, 0)), normal_map(new Texture(normal_map, 2)), roughness(new Texture(roughness, 1)) {
#ifdef IMGUI
	ImGuiManager::addObject(ImGuiObjectType::MATERIALS_MATERIAL_AR, this);
#endif
#ifdef DEBUG
	debug::NB_INSTANCES++;
#endif
}

Material_AR::~Material_AR() {
	delete albedo;
	delete normal_map;
	delete roughness;
#ifdef IMGUI
	ImGuiManager::removeObject(ImGuiObjectType::MATERIALS_MATERIAL_AR, this);
#endif
#ifdef DEBUG
	debug::NB_INSTANCES--;
#endif
}

void Material_AR::setup(Shader* shader) {
	switch (shader->getShaderType())
	{
	case ShaderType::DEFAULT_P_N_UV:
		albedo->bind();
		roughness->bind();
		normal_map->bind();
		shader->setUniform("albedo", 0);
		shader->setUniform("roughness", 1);
		shader->setUniform("normal_map", 2);
		break;
	default:
#ifdef CONSOLE
		std::cout << "[WARNING] Setup of Material_AR with shader " << shader->getShaderType() << " no setup is performed." << std::endl;
#endif
		break;
	}
}

#ifdef IMGUI
void* Material_AR::getAttribute() const {
	return new imGuiMaterial_AR_Attr(albedo, normal_map, roughness);
}

void Material_AR::updateAttribute(void* attr) const {

}

void Material_AR::setAttribute(void* attr) {

}

void Material_AR::imGuiPrintAttribute(void* attr) const {
	if (ImGui::TreeNode("albedo")) {
		albedo->imGuiPrintAttribute(ImGuiManager::getAttr(albedo));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("normal map")) {
		normal_map->imGuiPrintAttribute(ImGuiManager::getAttr(normal_map));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("roughness")) {
		roughness->imGuiPrintAttribute(ImGuiManager::getAttr(roughness));
		ImGui::TreePop();
	}
}

void Material_AR::deleteAttribute(void* attr) const {
	delete static_cast<imGuiMaterial_AR_Attr*>(attr);
}
#endif