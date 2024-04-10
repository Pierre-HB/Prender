#include "BasicScene.h"
#include "../algebra/Matrix4.h"
#include "../light/Light.h"
#include "../light/Light_Constant_Point.h"
#include "../../main.h"



BasicScene::BasicScene() {
    lightManager = new LightManager(vec4(0.1f, 0.2f, 0.1f, 1.0f));
    debug = new Texture("debug.jpeg", 0);
    texture = new Texture("container.jpg", 0);
    texture_smiley = new Texture("awesomeface.png", 2);

    debugMaterial = Texture::createDebugMaterial(512, 512, 1);
    //debugMaterial = Texture::createDebugMaterial(16, 16, 2);

    shader_test = new Shader(DEFAULT_P_N_UV);
    lightManager->bindShader(shader_test);
    
    camera = new PerspectiveCamera(800.0f / 600.0f, 3.14f/2, 0.1f, 1000.0f, translationMatrix(vec3(0, 0, 30)));

    object = new Object3D_P_N_UV("models/teapot.obj", "container.jpg", "roughness.png");
    //object = new Object3D_P_N_UV("models/teapot.obj", texture);


    lightManager->addLight(new Light_Constant_Point(vec3(0.8f, 1.0f, 0.5f), vec3(0.0f, 10, -15)));
    lightManager->addLight(new Light_Constant_Point(vec3(0.1f, 0.5f, 1.0f), vec3(0, -10, -15)));
    lightManager->addLight(new Light_Constant_Point(vec3(1.0f, 0.5f, 0.1f), vec3(-15, -10, -15)));

#ifdef DEBUG
    debug::NB_MAIN_INSTANCES++;
#endif
}

BasicScene::~BasicScene() {
    delete texture;
    delete texture_smiley;
    delete debug;
    delete shader_test;
    delete camera;
    delete object;
    delete lightManager;

    delete debugMaterial;

#ifdef DEBUG
    debug::NB_MAIN_INSTANCES--;
#endif
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
    /*object->setLightCasterID(lightManager->getLightCasters());*/
    //Light::loadLight(lightCasters);
    

    shader_test->use();
    lightManager->loadAmbiant(shader_test);
    object->setup(shader_test, camera->getProjectionMatrix(), camera->getViewMatrix());
    shader_test->setUniform("dimensions", ivec2(Engine::windowWidth, Engine::windowHeight));
    //texture_smiley->bind();
    //debugMaterial->bind();
    //shader_test->setUniform("roughness", 1); // TODO Create a Material class that will handle the albedo/normal map/material texture

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

    /*if (engine->toggleKeyState->forward) {
        std::vector<Light*> lights = lightManager->getLights();
        lightManager->removeLight(lights[0]);

        lights = lightManager->getLights();
    }*/

    object->setLightCasterID(lightManager->getLightCasters());
        
    
        
}