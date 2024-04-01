#include "BasicScene.h"
#include "../algebra/Matrix4.h"
#include <iostream>
#include "../light/Light.h"
#include "../light/Light_Constant_Point.h"


BasicScene::BasicScene() {
    lightManager = new LightManager(vec4(0.1, 0.2, 0.1, 1));
    //texture = new Texture("debug.jpeg", 0);
    texture = new Texture("container.jpg", 0);
    texture_smiley = new Texture("awesomeface.png", 1);

    shader_test = new Shader(DEFAULT_P_N_UV);
    lightManager->bindShader(shader_test);
    
    camera = new PerspectiveCamera(800.0f / 600.0f, 3.14f/2, 0.1f, 1000.0f);
    camera->moveWorld(translationMatrix(vec3(0, 0, 30)));

    object = new Object3D_P_N_UV("models/teapot.obj", texture);


    lightManager->addLight(new Light_Constant_Point(vec3(0.8, 1, 0.5), vec3(0, 10, -15)));
    lightManager->addLight(new Light_Constant_Point(vec3(0.1, 0.5, 1), vec3(0, -10, -15)));
    lightManager->addLight(new Light_Constant_Point(vec3(1, 0.5, 0.1), vec3(-15, -10, -15)));
}

BasicScene::~BasicScene() {
    //delete all pointers!!!
}

void BasicScene::render() {
    glClearColor(0.1f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    lightManager->computeLightCasters();
    lightManager->loadLight();

    //std::vector<lightCaster> lightCasters = Light::createLightCasterVertex(lights);
    //object->setLightCasterID(lightCasters);
    object->setLightCasterID(lightManager->getLightCasters());
    //Light::loadLight(lightCasters);
    

    shader_test->use();
    lightManager->loadAmbiant(shader_test);
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

    if (engine->toggleKeyState->forward) {
        std::vector<Light*> lights = lightManager->getLights();
        std::cout << lights.size() << " light alive" << std::endl;
        lightManager->removeLight(lights[0]);

        lights = lightManager->getLights();
        std::cout << lights.size() << " light alive after" << std::endl;
    }
        
    
        
}