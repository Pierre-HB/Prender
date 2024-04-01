#include "ImGuiManager.h"

#ifdef IMGUI

const char* imGuiGetObjectName(ImGuiObjectType type) {
	switch (type)
	{
	case ImGuiObjectType::LIGHT_CONSTANT_POINT:
		return "Light Point Constant";
	case ImGuiObjectType::OBJECT_OBJECT3D_DEFAULT_P_N_UV:
		return "Object P.N.UV";
	default:
		return "UNKOWN";
	}
}

void ImGuiObjectHierarchy::computeNbInstance() {
	nbInstance = 0;
	if (type != ImGuiObjectType::MaxObject)
		nbInstance = ImGuiManager::objects[static_cast<size_t>(type)].size();
	for (size_t i = 0; i < childs.size(); i++) {
		childs[i]->computeNbInstance();
		nbInstance += childs[i]->nbInstance;
	}
}

void ImGuiObjectHierarchy::render() {
	if (nbInstance == 0)
		ImGui::BeginDisabled();
	if (type != ImGuiObjectType::MaxObject) {
		for (size_t i = 0; i < ImGuiManager::objects[static_cast<size_t>(type)].size(); i++) {
			if (ImGui::Button(ImGuiManager::objects[static_cast<size_t>(type)][i].name.c_str())) {
				ImGuiManager::objects[static_cast<size_t>(type)][i].open = !ImGuiManager::objects[static_cast<size_t>(type)][i].open;
			}
		}
	}
	if (childs.size() != 0) {
		if (ImGui::TreeNode(name)) {
			for (size_t i = 0; i < childs.size(); i++) {
				childs[i]->render();
			}
			ImGui::TreePop();
		}
	}
	if (nbInstance == 0)
		ImGui::EndDisabled();
}


std::vector<std::vector<obj_attr>> ImGuiManager::objects = std::vector<std::vector<obj_attr>>(static_cast<size_t>(ImGuiObjectType::MaxObject));



ImGuiManager::ImGuiManager() {

	//light hierarchy ===========================================
	ImGuiObjectHierarchy* lightPointConstant = new ImGuiObjectHierarchy("Light Point Constant", ImGuiObjectType::LIGHT_CONSTANT_POINT);

	std::vector<ImGuiObjectHierarchy*> lightChilds = std::vector<ImGuiObjectHierarchy*>(1);
	lightChilds[0] = lightPointConstant;
	ImGuiObjectHierarchy* lightHierarchy = new ImGuiObjectHierarchy("Lights", lightChilds);


	//Object hierarchy ========================================
	ImGuiObjectHierarchy* object3dPNUV = new ImGuiObjectHierarchy("Object3D P N UV", ImGuiObjectType::OBJECT_OBJECT3D_DEFAULT_P_N_UV);

	std::vector<ImGuiObjectHierarchy*> object3DChilds = std::vector<ImGuiObjectHierarchy*>(1);
	object3DChilds[0] = object3dPNUV;
	ImGuiObjectHierarchy* Object3DHierarchy = new ImGuiObjectHierarchy("Object3D", object3DChilds);


	// Global Hierarchy =======================================
	std::vector<ImGuiObjectHierarchy*> globalChilds = std::vector<ImGuiObjectHierarchy*>(2);
	globalChilds[0] = lightHierarchy;
	globalChilds[1] = Object3DHierarchy;

	hierarchy = new ImGuiObjectHierarchy("Instances", globalChilds);

}


ImGuiManager::~ImGuiManager() {

}

void ImGuiManager::addObject(ImGuiObjectType type, ImGuiPrintable* obj) {
	std::string name = imGuiGetObjectName(type) + std::string(" ") + std::to_string(ImGuiManager::objects[static_cast<size_t>(type)].size());

	ImGuiManager::objects[static_cast<size_t>(type)].push_back(obj_attr(obj, obj->getAttribute(), name));

}

//TO TEST
void ImGuiManager::removeObject(ImGuiObjectType type, ImGuiPrintable* obj) {
	for (size_t i = 0; i < ImGuiManager::objects[static_cast<size_t>(type)].size(); i++) {
		if (ImGuiManager::objects[static_cast<size_t>(type)][i].obj == obj) {
			ImGuiManager::objects[static_cast<size_t>(type)].erase(ImGuiManager::objects[static_cast<size_t>(type)].begin() + i);
			return;
		}
	}
}

void ImGuiManager::setAttributes() {
	for (size_t i = 0; i < objects.size(); i++) {
		for (size_t j = 0; j < objects[i].size(); j++) {
			objects[i][j].obj->setAttribute(objects[i][j].attr);
		}
	}
}

void ImGuiManager::updateAttributes() {
	for (size_t i = 0; i < objects.size(); i++) {
		for (size_t j = 0; j < objects[i].size(); j++) {
			objects[i][j].obj->updateAttribute(objects[i][j].attr);
		}
	}
}

void ImGuiManager::render() {
	/*ImGui::Begin("test tree");
	if (ImGui::TreeNode("root")) {
		ImGui::Text("test");
		if (ImGui::TreeNode("child1")) {
			ImGui::Text("child 1");
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("child2")) {
			ImGui::Text("child 2");
			ImGui::Begin("test child 2");
			ImGui::Text("C");
			ImGui::End();
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::End();*/
	//int i = 0;
	//while (i != static_cast<int>(ImGuiObjectType::MaxObject)) {
	//	//TODO
	//	if(ImGui::TreeNode(imGuiGetObjectName(i)))
	//	i++;
	//}

	/*if (ImGui::TreeNode("Instances")) {
		if (ImGui::TreeNode("Lights")) {
			if(ImGui::TreeNode("Point Light")) {
				for (size_t j = 0; j < objects[static_cast<size_t>(ImGuiObjectType::LIGHT_CONSTANT_POINT)].size(); j++) {
					if (ImGui::Button(objects[static_cast<size_t>(ImGuiObjectType::LIGHT_CONSTANT_POINT)][j].name.c_str())) {
						objects[static_cast<size_t>(ImGuiObjectType::LIGHT_CONSTANT_POINT)][j].open = true;
					}
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}*/
	hierarchy->computeNbInstance();
	hierarchy->render();

	for (size_t i = 0; i < objects.size(); i++) {
		for (size_t j = 0; j < objects[i].size(); j++) {
			if (objects[i][j].open) {
				ImGui::Begin(objects[i][j].name.c_str(), &objects[i][j].open);
				objects[i][j].obj->imGuiPrintAttribute(objects[i][j].attr);
				ImGui::End();
			}
		}
	}
}




#endif