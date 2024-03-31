#include "BasicScene.h"
#include "../algebra/Matrix4.h"
#include <iostream>


BasicScene::BasicScene() {
    //texture = new Texture("debug.jpeg", 0);
    texture = new Texture("container.jpg", 0);
    texture_smiley = new Texture("awesomeface.png", 1);

    shader_test = new Shader(DEFAULT_P_N_UV);
    
    camera = new PerspectiveCamera(800.0f / 600.0f, 3.14f/2, 0.1f, 1000.0f);
    camera->moveWorld(translationMatrix(vec3(0, 0, 30)));

    object = new Object3D_P_N_UV("models/teapot.obj", texture);
    //object = new Object3D_P_N_UV("models/cube.obj", texture);
}

BasicScene::~BasicScene() {

}

void BasicScene::render() {
    glClearColor(0.1f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader_test->use();
    object->setup(shader_test, camera->getProjectionMatrix(), camera->getViewMatrix());

    texture_smiley->bind();
    shader_test->setUniform("smiley", 1);

    object->draw();

    
}

void BasicScene::update(Engine* engine) {
    float a = static_cast<float>(2 * 3.1415 / (10 * engine->get_tps())); // 1/10 tour/s
    float s = static_cast<float>(2 / (engine->get_tps())); // 1m/s

    object->moove(rotationMatrixY(a));
    if (engine->currentKeyState->forward) 
        camera->moveView(translationMatrix(vec3(0, 0, -s)));
    if (engine->currentKeyState->backward)
        camera->moveView(translationMatrix(vec3(0, 0, s)));
    if (engine->currentKeyState->left)
        camera->moveView(translationMatrix(vec3(-s, 0, 0)));
    if (engine->currentKeyState->right)
        camera->moveView(translationMatrix(vec3(s, 0, 0)));
    
        
}