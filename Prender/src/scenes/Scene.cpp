#include "scene.h"

Scene::Scene() {
#ifdef DEBUG
	debug::NB_MAIN_INSTANCES++;
#endif
}

Scene::~Scene() {
#ifdef DEBUG
	debug::NB_MAIN_INSTANCES--;
#endif
}

void Scene::update(Engine* engine) {
#ifdef CONSOLE
	std::cout << "[WARNING] Updating default Scene. No update operation is performed" << std::endl;
#endif
}

void Scene::render() {
#ifdef CONSOLE
	std::cout << "WARNING, redering default Scene. No render operation is performed" << std::endl;
#endif
}

