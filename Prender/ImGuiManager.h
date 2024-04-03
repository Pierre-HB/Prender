#pragma once

#include "main.h"
#include <vector>
#include <string>


#ifdef IMGUI
struct ImGuiObjectHierarchy;
class ImGuiPrintable;
class ImGuiManager;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include "src/light/Light_Constant_Point.h"
class ImGuiPrintable;


enum class ImGuiObjectType { LIGHT_CONSTANT_POINT, OBJECT_OBJECT3D_DEFAULT_P_N_UV, MaxObject };




struct ImGuiObjectHierarchy {
	const char* name;
	std::vector<ImGuiObjectHierarchy*> childs;
	ImGuiObjectType type;
	size_t nbInstance;

	//! node intialisation
	ImGuiObjectHierarchy(const char* name, std::vector<ImGuiObjectHierarchy*> childs) : name(name), childs(childs), type(ImGuiObjectType::MaxObject), nbInstance() {}
	
	ImGuiObjectHierarchy(const char* name, ImGuiObjectType type) : name(name), childs(), type(type), nbInstance() {}

	void computeNbInstance();

	void render();
};





const char* imGuiGetObjectName(ImGuiObjectType type);

struct obj_attr
{
	ImGuiPrintable* obj;
	void* attr;
	std::string name;
	bool open;

	obj_attr() : obj(nullptr), attr(nullptr), name(""), open(false) {}

	obj_attr(ImGuiPrintable* obj, void* attr, std::string name) : obj(obj), attr(attr), name(name), open(false) {}

};

//#include "src/light/Light_Constant_Point.h"
class ImGuiPrintable
{
public:

	//! create the attribute struct and return a pointer to it
	virtual void* getAttribute() const = 0;

	//! get a pointer to an attribut struct and update it
	virtual void updateAttribute(void* attr) const = 0;

	//! get a pointer to an attribute struct and actualise private variable to match it
	virtual void setAttribute(void* attr) = 0;

	//! print the attribut in ImGui
	virtual void imGuiPrintAttribute(void* attr) const = 0;
};

class ImGuiManager
{
private:
	static std::vector<std::vector<obj_attr>> objects;
	ImGuiObjectHierarchy* hierarchy;
public:
	ImGuiManager();

	~ImGuiManager();
	
	static void addObject(ImGuiObjectType type, ImGuiPrintable* obj, std::string name);
	static void addObject(ImGuiObjectType type, ImGuiPrintable* obj);

	static void removeObject(ImGuiObjectType type, ImGuiPrintable* obj);

	//! to call at start of update loop to insert modified attributes (by ImGUI) in the code
	void setAttributes();

	//! to call at the end of update loop to update the attributes after the loop's changes
	void updateAttributes();

	//! to call during render phase to update attributes in ImGui
	void renderInstancesTree();
	void renderInstances();

	//! To call at the begening of the render pass to initialise the on screen console
	void beginConsole() const;

	//! To call at the end of the render pass to close the on screen console
	void endConsole() const;

	static void startFrame();

	static void endFrame();

	static void initializeContext(GLFWwindow* window);

	static void destroyContext();

	friend void ImGuiObjectHierarchy::computeNbInstance();
	friend void ImGuiObjectHierarchy::render();
};



#endif