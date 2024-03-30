#include "Object3D.h"
#include <iostream>

void Object3D::moove(const mat4& transformation) {
	world = world * transformation;
}

void Object3D::draw() {
	std::cout << "[WARNING] draw of Object3D, no draw is performed." << std::endl;
}