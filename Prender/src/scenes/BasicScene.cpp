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
    shader_tbn = new Shader(DEBUG_TBN);
    lightManager->bindShader(shader_test);
    
    camera = new PerspectiveCamera(800.0f / 600.0f, 3.14f/2, 0.1f, 1000.0f, translationMatrix(vec3(0, 0, 30)));

    object = new Object3D_P_N_UV("models/teapot.obj", "container.jpg", "roughness.png");
    //object = new Object3D_P_N_UV("models/teapot.obj", texture);


    lightManager->addLight(new Light_Constant_Point(vec3(0.8f, 1.0f, 0.5f), vec3(0.0f, 10, -15)));
    lightManager->addLight(new Light_Constant_Point(vec3(0.1f, 0.5f, 1.0f), vec3(0, -10, -15)));
    lightManager->addLight(new Light_Constant_Point(vec3(1.0f, 0.5f, 0.1f), vec3(-15, -10, -15)));

    showNormal = false;
    showWireframe = false;
    showMaterial = true;
    rotation = true;
    backCulling = true;

#ifdef DEBUG
    debug::NB_MAIN_INSTANCES++;
#endif

#ifdef IMGUI
    ImGuiManager::addObject(ImGuiObjectType::SCENE_BASIC_SCENE, this);
#endif
}

BasicScene::~BasicScene() {
    delete texture;
    delete texture_smiley;
    delete debug;
    delete shader_test;
    delete shader_tbn;
    delete camera;
    delete object;
    delete lightManager;

    delete debugMaterial;

#ifdef DEBUG
    debug::NB_MAIN_INSTANCES--;
#endif

#ifdef IMGUI
    ImGuiManager::removeObject(ImGuiObjectType::SCENE_BASIC_SCENE, this);
#endif
}

void BasicScene::render() {
    glClearColor(0.1f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(backCulling)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    lightManager->computeLightCasters();
    lightManager->loadLight();
    
    //shader_test->setUniform("dimensions", ivec2(Engine::windowWidth, Engine::windowHeight));

    if (showMaterial) {
        shader_test->use();
        lightManager->loadAmbiant(shader_test);
        object->setup(shader_test, camera->getProjectionMatrix(), camera->getViewMatrix());
        object->draw();
    }

    if (showWireframe) {
        // dirty but working
        shader_test->use();
        object->setup(shader_test, camera->getProjectionMatrix(), camera->getViewMatrix());
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(0, -125);
        object->draw();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glPolygonOffset(0, 0);
    }
    
    
    
    if (showNormal) {
        shader_tbn->use();
        object->setup(shader_tbn, camera->getProjectionMatrix(), camera->getViewMatrix());
        object->draw();
    }
    

}

void BasicScene::update(Engine* engine) {
    float a = static_cast<float>(2 * 3.1415 / (10 * engine->get_tps())); // 1/10 tour/s
    float s = static_cast<float>(2 / (engine->get_tps())); // 1m/s

    if(rotation)
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

#ifdef IMGUI

void* BasicScene::getAttribute() const {
    return new imGuiBasicSceneAttr(showNormal, showWireframe, showMaterial, rotation, backCulling);
}

void BasicScene::updateAttribute(void* attr) const{
    static_cast<imGuiBasicSceneAttr*>(attr)->showNormal = showNormal;
    static_cast<imGuiBasicSceneAttr*>(attr)->showWireframe = showWireframe;
    static_cast<imGuiBasicSceneAttr*>(attr)->showMaterial = showMaterial;
    static_cast<imGuiBasicSceneAttr*>(attr)->rotation = rotation;
    static_cast<imGuiBasicSceneAttr*>(attr)->backCulling = backCulling;
}

void BasicScene::setAttribute(void* attr){
    showNormal = static_cast<imGuiBasicSceneAttr*>(attr)->showNormal;
    showWireframe = static_cast<imGuiBasicSceneAttr*>(attr)->showWireframe;
    showMaterial = static_cast<imGuiBasicSceneAttr*>(attr)->showMaterial;
    rotation = static_cast<imGuiBasicSceneAttr*>(attr)->rotation;
    backCulling = static_cast<imGuiBasicSceneAttr*>(attr)->backCulling;
}

void BasicScene::imGuiPrintAttribute(void* attr) const{
    ImGui::Checkbox("normal", &static_cast<imGuiBasicSceneAttr*>(attr)->showNormal);
    ImGui::Checkbox("wireframe", &static_cast<imGuiBasicSceneAttr*>(attr)->showWireframe);
    ImGui::Checkbox("default", &static_cast<imGuiBasicSceneAttr*>(attr)->showMaterial);
    ImGui::Checkbox("rotation", &static_cast<imGuiBasicSceneAttr*>(attr)->rotation);
    ImGui::Checkbox("backCulling", &static_cast<imGuiBasicSceneAttr*>(attr)->backCulling);
    }

void BasicScene::deleteAttribute(void* attr) const{
    delete static_cast<imGuiBasicSceneAttr*>(attr);
}

#endif