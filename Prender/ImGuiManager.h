#pragma once

#include "main.h"
#include <vector>
#include <string>


#ifdef IMGUI
//! node of the instance tree
struct ImGuiObjectHierarchy;
//! virtual class defining the neccessary method to render the ui of an object
class ImGuiPrintable;
//! the UI manager
class ImGuiManager;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//! types of object (to put them in the right place in the instance tree)
enum class ImGuiObjectType { LIGHT_CONSTANT_POINT, OBJECT_OBJECT3D_DEFAULT_P_N_UV, UTILS_PERSPECTIVE_CAMERA, UTILS_TEXTURE, MaxObject };



//! node of the instance tree
struct ImGuiObjectHierarchy {
	const char* name;
	std::vector<ImGuiObjectHierarchy*> childs;
	ImGuiObjectType type;
	size_t nbInstance;

	//! Node
	ImGuiObjectHierarchy(const char* name, std::vector<ImGuiObjectHierarchy*> childs) : name(name), childs(childs), type(ImGuiObjectType::MaxObject), nbInstance() {
#ifdef DEBUG
		debug::NB_INSTANCES++;
#endif
	}
	
	//! Leaf
	ImGuiObjectHierarchy(const char* name, ImGuiObjectType type) : name(name), childs(), type(type), nbInstance() {
#ifdef DEBUG
		debug::NB_INSTANCES++;
#endif
	}

	~ImGuiObjectHierarchy() {
		for (size_t i = 0; i < childs.size(); i++)
			delete childs[i];
#ifdef DEBUG
		debug::NB_INSTANCES--;
#endif
	}

	//! compute the number of instance in this category
	void computeNbInstance();

	//! draw the ui
	void render();
};

//! virtual class defining the neccessary method to render the ui of an object
class ImGuiPrintable
{
public:

	//! create the attribute struct and return a pointer to it
	virtual void* getAttribute() const = 0;

	//! destroy the attribut struct
	virtual void deleteAttribute(void* attr) const = 0;

	//! get a pointer to an attribut struct and update it
	virtual void updateAttribute(void* attr) const = 0;

	//! get a pointer to an attribute struct and actualise private variable to match it
	virtual void setAttribute(void* attr) = 0;

	//! print the attribut in ImGui
	virtual void imGuiPrintAttribute(void* attr) const = 0;
};


//! give a category name to each object type
const char* imGuiGetObjectName(ImGuiObjectType type);
const char* imGuiGetObjectName(int typeNumber);


//! gather an object, it's attributes, it's name in the instance tree and a boolean saying if it's rendered or not
struct obj_attr
{
	ImGuiPrintable* obj;
	void* attr;
	std::string name;
	bool open;

	obj_attr() : obj(nullptr), attr(nullptr), name(""), open(false) {
#ifdef DEBUG
		debug::NB_ATTR++;
#endif
	}

	obj_attr(ImGuiPrintable* obj, void* attr, std::string name) : obj(obj), attr(attr), name(name), open(false) {
#ifdef DEBUG
		debug::NB_ATTR++;
#endif
	}

	~obj_attr() {
		if (attr != nullptr)
			obj->deleteAttribute(attr);
			
#ifdef DEBUG
		debug::NB_ATTR--;
#endif
	}

};

//! The ui manager
class ImGuiManager
{
private:
	static std::vector<std::vector<obj_attr*>> objects;
	ImGuiObjectHierarchy* hierarchy;
public:
	//! constructor
	ImGuiManager();

	~ImGuiManager();
	
	//! add an object in the instance tree
	static void addObject(ImGuiObjectType type, ImGuiPrintable* obj, std::string name);

	//! add an object in the instance tree
	static void addObject(ImGuiObjectType type, ImGuiPrintable* obj);

	//! remove an object frome the instance tree
	static void removeObject(ImGuiObjectType type, ImGuiPrintable* obj);

	//! to call at start of update loop to insert modified attributes (by ImGUI) in the code
	void setAttributes();

	//! to call at the end of update loop to update the attributes after the loop's changes
	void updateAttributes();

	//! Draw the instnce tree
	void renderInstancesTree();

	//! draw the ui of each opened object
	void renderInstances();

	//! To call at the begening of the render pass to initialise the on screen console
	void beginConsole() const;

	//! To call at the end of the render pass to close the on screen console
	void endConsole() const;

	//! clear the ImGui frame
	static void startFrame();

	//! draw the ImGui frame
	static void endFrame();

	//! initialize the IGui context
	static void initializeContext(GLFWwindow* window);

	//! destroy the ImGui context
	static void destroyContext();

	friend void ImGuiObjectHierarchy::computeNbInstance();
	friend void ImGuiObjectHierarchy::render();
};



#endif