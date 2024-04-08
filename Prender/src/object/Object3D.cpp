#include "Object3D.h"

void Object3D::moove(const mat4& transformation) {
	world = transformation * world;
}

void Object3D::draw() {
#ifdef CONSOLE
	std::cout << "[WARNING] draw of Object3D, no draw is performed." << std::endl;
#endif
}

#ifdef IMGUI
void* Object3D::getAttribute() const {
	return new imGuiObject3DAttr(object, world);
}

void Object3D::updateAttribute(void* attr) const {
	static_cast<imGuiObject3DAttr*>(attr)->object.updateAttr(object);
	static_cast<imGuiObject3DAttr*>(attr)->world.updateAttr(world);
}

void Object3D::setAttribute(void* attr) {
	object = static_cast<imGuiObject3DAttr*>(attr)->object.getTansformation();
	world = static_cast<imGuiObject3DAttr*>(attr)->world.getTansformation();
}

void Object3D::imGuiPrintAttribute(void* attr) const {
	static_cast<imGuiObject3DAttr*>(attr)->object.imGuiPrintAttribute();
	static_cast<imGuiObject3DAttr*>(attr)->world.imGuiPrintAttribute();
}

void Object3D::deleteAttribute(void* attr) const {
	delete static_cast<imGuiObject3DAttr*>(attr);
}
#endif