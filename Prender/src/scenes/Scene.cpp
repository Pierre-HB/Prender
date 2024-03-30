#include "scene.h"
#include <iostream>

Scene::Scene() {

}

Scene::~Scene() {

}

void Scene::update(Engine* engine) {
	std::cout << "WARNING, updating default Scene. No update operation is performed" << std::endl;
}

void Scene::render() {
	std::cout << "WARNING, redering default Scene. No render operation is performed" << std::endl;
}

