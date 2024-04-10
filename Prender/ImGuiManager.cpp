#include "ImGuiManager.h"

#ifdef IMGUI
#include "src/imgui/imgui_impl_opengl3.h"
#include "src/imgui/imgui_impl_glfw.h"

const char* imGuiGetObjectName(ImGuiObjectType type) {
	switch (type)
	{
	case ImGuiObjectType::LIGHT_CONSTANT_POINT:
		return "Light Point Constant";
	case ImGuiObjectType::OBJECT_OBJECT3D_DEFAULT_P_N_UV:
		return "Object P.N.UV";
	case ImGuiObjectType::UTILS_PERSPECTIVE_CAMERA:
		return "Perspective Camera";
	case ImGuiObjectType::UTILS_TEXTURE:
		return "Texture";
	case ImGuiObjectType::MATERIALS_MATERIAL_AR:
		return "Material AR";
	case ImGuiObjectType::SCENE_BASIC_SCENE:
		return "Basic Scene";
	default:
		return "UNKOWN";
	}
}

const char* imGuiGetObjectName(int typeNumber) {
	switch (typeNumber)
	{
	case static_cast<int>(ImGuiObjectType::LIGHT_CONSTANT_POINT):
		return "Light Point Constant";
	case static_cast<int>(ImGuiObjectType::OBJECT_OBJECT3D_DEFAULT_P_N_UV):
		return "Object P.N.UV";
	case static_cast<int>(ImGuiObjectType::UTILS_PERSPECTIVE_CAMERA):
		return "Perspective Camera";
	case static_cast<int>(ImGuiObjectType::UTILS_TEXTURE):
		return "Texture";
	case static_cast<int>(ImGuiObjectType::MATERIALS_MATERIAL_AR):
		return "Material";
	case static_cast<int>(ImGuiObjectType::SCENE_BASIC_SCENE):
		return "Basic Scene";
	default:
		return "UNKOWN";
	}
}

bool ImGuiObjectHierarchy::addChild(ImGuiObjectHierarchy* child, const char* parentName) {
	if (strcmp(parentName, name) == 0) {
		childs.push_back(child);
		return true;
	}
	for (size_t i = 0; i < childs.size(); i++) {
		if (childs[i]->addChild(child, parentName))
			return true;
	}
	return false;
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
			if (ImGui::Button(ImGuiManager::objects[static_cast<size_t>(type)][i]->name.c_str())) {
				ImGuiManager::objects[static_cast<size_t>(type)][i]->open = !ImGuiManager::objects[static_cast<size_t>(type)][i]->open;
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

void ImGuiManager::beginConsole() const {
	ImGui::Begin("Console");
}

void ImGuiManager::endConsole() const {
	ImGui::End();
}

void ImGuiManager::startFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::endFrame() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::initializeContext(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void ImGuiManager::destroyContext() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

std::vector<std::vector<obj_attr*>> ImGuiManager::objects = std::vector<std::vector<obj_attr*>>(static_cast<size_t>(ImGuiObjectType::MaxObject));



ImGuiManager::ImGuiManager() {

	hierarchy = new ImGuiObjectHierarchy("Instances");
	hierarchy->addChild(new ImGuiObjectHierarchy("Lights"), "Instances");
	hierarchy->addChild(new ImGuiObjectHierarchy("Object3D"), "Instances");
	hierarchy->addChild(new ImGuiObjectHierarchy("Camera"), "Instances");
	hierarchy->addChild(new ImGuiObjectHierarchy("Texture"), "Instances");
	hierarchy->addChild(new ImGuiObjectHierarchy("Material"), "Instances");
	hierarchy->addChild(new ImGuiObjectHierarchy("Scene", ImGuiObjectType::SCENE_BASIC_SCENE), "Instances");

	
	hierarchy->addChild(new ImGuiObjectHierarchy("Light Point Constant", ImGuiObjectType::LIGHT_CONSTANT_POINT), "Lights");
	hierarchy->addChild(new ImGuiObjectHierarchy("Object3D P N UV", ImGuiObjectType::OBJECT_OBJECT3D_DEFAULT_P_N_UV), "Object3D");
	hierarchy->addChild(new ImGuiObjectHierarchy("Perspective Camera", ImGuiObjectType::UTILS_PERSPECTIVE_CAMERA), "Camera");
	hierarchy->addChild(new ImGuiObjectHierarchy("Texture", ImGuiObjectType::UTILS_TEXTURE), "Texture");
	hierarchy->addChild(new ImGuiObjectHierarchy("Material AR", ImGuiObjectType::MATERIALS_MATERIAL_AR), "Material");
	
#ifdef DEBUG
	debug::NB_MAIN_INSTANCES++;
#endif
}


ImGuiManager::~ImGuiManager() {
	delete hierarchy;
#ifdef CONSOLE
	for (size_t i = 0; i < objects.size(); i++) {
		if (objects[i].size() != 0)
			std::cout << "[WARNING] there are still " << objects[i].size() << " object of type " << imGuiGetObjectName(static_cast<int>(i)) << " alive." << std::endl;
	}
#endif
#ifdef DEBUG
	debug::NB_MAIN_INSTANCES--;
#endif	
}

void ImGuiManager::addObject(ImGuiObjectType type, ImGuiPrintable* obj) {
	std::string name = imGuiGetObjectName(type) + std::string(" ") + std::to_string(ImGuiManager::objects[static_cast<size_t>(type)].size());

	ImGuiManager::addObject(type, obj, name);
}

void ImGuiManager::addObject(ImGuiObjectType type, ImGuiPrintable* obj, std::string name) {
	ImGuiManager::objects[static_cast<size_t>(type)].push_back(new obj_attr(obj, obj->getAttribute(), name));

}

void ImGuiManager::removeObject(ImGuiObjectType type, ImGuiPrintable* obj) {
	for (size_t i = 0; i < ImGuiManager::objects[static_cast<size_t>(type)].size(); i++) {
		if (ImGuiManager::objects[static_cast<size_t>(type)][i]->obj == obj) {
			delete objects[static_cast<size_t>(type)][i];
			ImGuiManager::objects[static_cast<size_t>(type)].erase(ImGuiManager::objects[static_cast<size_t>(type)].begin() + i);

			return;
		}
	}
}

void* ImGuiManager::getAttr(void* obj) {
	for (size_t i = 0; i < objects.size(); i++) {
		for (size_t j = 0; j < objects[i].size(); j++) {
			if (objects[i][j]->obj == obj)
				return objects[i][j]->attr;
		}
	}
	return nullptr;
}

void ImGuiManager::setAttributes() {
	for (size_t i = 0; i < objects.size(); i++) {
		for (size_t j = 0; j < objects[i].size(); j++) {
			objects[i][j]->obj->setAttribute(objects[i][j]->attr);
		}
	}
}

void ImGuiManager::updateAttributes() {
	for (size_t i = 0; i < objects.size(); i++) {
		for (size_t j = 0; j < objects[i].size(); j++) {
			objects[i][j]->obj->updateAttribute(objects[i][j]->attr);
		}
	}
}

void ImGuiManager::renderInstancesTree() {

	hierarchy->computeNbInstance();
	hierarchy->render();
}

void ImGuiManager::renderInstances() {
	for (size_t i = 0; i < objects.size(); i++) {
		for (size_t j = 0; j < objects[i].size(); j++) {
			if (objects[i][j]->open) {
				ImGui::Begin(objects[i][j]->name.c_str(), &objects[i][j]->open);
				objects[i][j]->obj->imGuiPrintAttribute(objects[i][j]->attr);
				ImGui::End();
			}
		}
	}
}




#endif