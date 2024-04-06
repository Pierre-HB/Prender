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

	//Camera hierarchy =========================================
	ImGuiObjectHierarchy* perspectiveCamera = new ImGuiObjectHierarchy("Perspective Camera", ImGuiObjectType::UTILS_PERSPECTIVE_CAMERA);

	std::vector<ImGuiObjectHierarchy*> cameraChilds = std::vector<ImGuiObjectHierarchy*>(1);
	cameraChilds[0] = perspectiveCamera;
	ImGuiObjectHierarchy* cameraHierarchy = new ImGuiObjectHierarchy("Camera", cameraChilds);

	//Texture hierarchy =========================================
	ImGuiObjectHierarchy* texture = new ImGuiObjectHierarchy("Texture", ImGuiObjectType::UTILS_TEXTURE);

	std::vector<ImGuiObjectHierarchy*> textureChilds = std::vector<ImGuiObjectHierarchy*>(1);
	textureChilds[0] = texture;
	ImGuiObjectHierarchy* textureHierarchy = new ImGuiObjectHierarchy("Texture", textureChilds);


	// Global Hierarchy =======================================
	std::vector<ImGuiObjectHierarchy*> globalChilds = std::vector<ImGuiObjectHierarchy*>(4);
	globalChilds[0] = lightHierarchy;
	globalChilds[1] = Object3DHierarchy;
	globalChilds[2] = cameraHierarchy;
	globalChilds[3] = textureHierarchy;

	hierarchy = new ImGuiObjectHierarchy("Instances", globalChilds);

}


ImGuiManager::~ImGuiManager() {

}

void ImGuiManager::addObject(ImGuiObjectType type, ImGuiPrintable* obj) {
	std::string name = imGuiGetObjectName(type) + std::string(" ") + std::to_string(ImGuiManager::objects[static_cast<size_t>(type)].size());

	ImGuiManager::addObject(type, obj, name);
}

void ImGuiManager::addObject(ImGuiObjectType type, ImGuiPrintable* obj, std::string name) {

	ImGuiManager::objects[static_cast<size_t>(type)].push_back(obj_attr(obj, obj->getAttribute(), name));

}

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

void ImGuiManager::renderInstancesTree() {

	hierarchy->computeNbInstance();
	hierarchy->render();
}

void ImGuiManager::renderInstances() {
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