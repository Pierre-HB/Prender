#include "scene.h"

Scene::Scene() {

}

Scene::~Scene() {

}

void Scene::update(Engine* engine) {
#ifdef CONSOLE
	std::cout << "WARNING, updating default Scene. No update operation is performed" << std::endl;
#endif
}

void Scene::render() {
#ifdef CONSOLE
	std::cout << "WARNING, redering default Scene. No render operation is performed" << std::endl;
#endif
}

