#include "Light.h"

Light::Light(const vec3& lightColor, float falloff, const vec3& position, float cos_angle_min, const vec3& direction, float cos_angle_max) : lightColor(lightColor), falloff(falloff), position(position), cos_angle_min(cos_angle_min), direction(direction), cos_angle_max(cos_angle_max) {

}

Light::Light() : lightColor(), falloff(-1.0f), position(), cos_angle_min(), direction(), cos_angle_max() {

}

Light::~Light() {
}

lightCaster Light::getLightCaster() const {
	return lightCaster(lightColor, falloff, position, cos_angle_min, direction, cos_angle_max);
}


LightManager::LightManager(const vec4& ambiantColor) : lightCatserUniformBuffer(), ambiantColor(ambiantColor) {
    lightBindingPoint = 0;
    lightCasters = std::vector<lightCaster>(MAX_LIGHT);
    //create the lightCasterBuffer
    glGenBuffers(1, &lightCatserUniformBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, lightCatserUniformBuffer);
    glBufferData(GL_UNIFORM_BUFFER, MAX_LIGHT * sizeof(lightCaster), NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, lightBindingPoint, lightCatserUniformBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

LightManager::~LightManager() {
    glDeleteBuffers(1, &lightCatserUniformBuffer);
}

void LightManager::addLight(Light* light) {
    lights.push_back(light);
    if (lights.size() > MAX_LIGHT)
        std::cout << "[WARNING], lightManager has more than " << MAX_LIGHT << ", some light will not be rendered." << std::endl;
}

//! TO TEST
void LightManager::removeLight(Light* light) {
    std::cout << "remove light. initial : " << lights.size();
    for (size_t i = 0; i < lights.size(); i++) {
        if (lights[i] == light) {
            delete lights[i];
            lights.erase(lights.begin() + i);
            break;
        }
    }
    //remove(lights.begin(), lights.end(), light);
    std::cout << " final : " << lights.size() << std::endl;
}

std::vector<Light*> LightManager::getLights() {
    return lights;
}

void LightManager::setAmbiantColor(const vec4& color) {
    ambiantColor = color;
}

void LightManager::computeLightCasters() {
    if (lights.size() > MAX_LIGHT)
        std::cout << "[WARNING] trying to load more than " << MAX_LIGHT << " lights.Only " << MAX_LIGHT << " lights are loaded." << std::endl;

    for (int i = 0; i < MAX_LIGHT; i++) {
        lightCasters[i] = i < lights.size() ? lights[i]->getLightCaster() : lightCaster();
    }
}

std::vector<lightCaster> LightManager::getLightCasters() const {
    return lightCasters;
}

void LightManager::bindShader(Shader* shader) const {
    switch (shader->getShaderType())
    {
    case NO_SHADER:
        break;
    case DEFAULT_P_N_UV:
        glUniformBlockBinding(shader->getID(), glGetUniformBlockIndex(shader->getID(), "lightCastersBlock"), lightBindingPoint);
        break;
    default:
        std::cout << "Binding Light to Unknown Shader : " << shader->getShaderType() << std::endl;
        break;
    }
}

void LightManager::loadLight() const {
    if (lightCasters.size() != MAX_LIGHT)
        std::cout << "[WARNING] Loading wrong number of lights. Should be " << MAX_LIGHT << " lights ans it's : " << lightCasters.size() << std::endl;

    glBindBuffer(GL_UNIFORM_BUFFER, lightCatserUniformBuffer);
    glBufferData(GL_UNIFORM_BUFFER, MAX_LIGHT * sizeof(lightCaster), lightCasters.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightManager::loadAmbiant(Shader* shader) const {
    shader->setUniform("ambiantColor", ambiantColor);
}