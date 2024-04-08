#pragma once

#include <glad/glad.h>
#include <vector>

#include "../algebra/Algebra.h"
#include "../../Shader.h"

//! Light representation in the GPU memory
struct lightCaster
{
	vec3 lightColor; //! color of the light. in HDR color, might be heigher than 1
	float falloff; //! the fallof of the intensity
	//! 0 = no falloff
	//! 1 = falloff in 1/d
	//! 2 = falloff in 1/d^2
	//! supposed to be integer but can actually be floating
	//! negative value means NO LIGHT
	vec3 position; //! position in world or direction of light, dependng on the light type
	float cos_angle_min; //! for spotLights 
	vec3 direction;//! direction of light
	float cos_angle_max;//! for spotLights

	//! Regular construction of a light
	lightCaster(const vec3& lightColor, float falloff, const vec3& position, float cos_angle_min, const vec3& direction, float cos_angle_max) : lightColor(lightColor), falloff(falloff), position(position), cos_angle_min(cos_angle_min), direction(direction), cos_angle_max(cos_angle_max) {}

	//! Empty constructor for a fake light
	lightCaster() : lightColor(), falloff(-1.0f), position(), cos_angle_min(), direction(), cos_angle_max() {}

};

//! General class for lights
class Light
{
protected:

	vec3 lightColor; //! color of the light. in HDR color, might be heigher than 1
	float falloff; //! the fallof of the intensity
	//! 0 = no falloff
	//! 1 = falloff in 1/d
	//! 2 = falloff in 1/d^2
	//! supposed to be integer but can actually be floating
	//! negative value means NO LIGHT
	vec3 position; //! position in world or direction of light, dependng on the light type
	float cos_angle_min; //! for spotLights 
	vec3 direction;//! direction of light
	float cos_angle_max;//! for spotLights

public:

	//! empty light
	Light();

	//! general light constructor
	Light(const vec3& lightColor, float falloff, const vec3& position, float cos_angle_min, const vec3& direction, float cos_angle_max);

	virtual ~Light();

	//! return the lighCaster data to load in OpenGL
	virtual lightCaster getLightCaster() const;
};

//! Class to handle several lights.
//! In charge of copying the lightsData on the GPU
//! In charge of computing wich objects is affected by wich lights
class LightManager
{
private:
	const int MAX_LIGHT = 32;//! Max number of light in the scene. IF CHANGED MUST ALSO BE CHANGED IN THE SHADERS CODE

	int lightBindingPoint;//! binding point of the lightsBuffer uniform in the GPU memory
	GLuint lightCatserUniformBuffer;//! ID of the lights buffer in the GPU (Maybe use a dedicated class UB for uniforms buffers)
	std::vector<Light*> lights; //! list of all lights in the scene
	std::vector<lightCaster> lightCasters;//! lights data to sent to the gpu
	vec4 ambiantColor;//! ambiant color

public:
	//! constructor with an ambiant lighht
	LightManager(const vec4& ambiantColor=vec4());

	~LightManager();

	//! add a light in the scene
	void addLight(Light* light);

	//! remove a light in the scene
	void removeLight(Light* light);

	//! get all lights in the scene
	std::vector<Light*> getLights();

	//! set the ambiant color of the scene
	void setAmbiantColor(const vec4& color);

	//! Compute the lightCasters variable wich is supposed to be sent in the GPU after.
	void computeLightCasters();

	//! Getter lightCasters
	std::vector<lightCaster> getLightCasters() const;

	//! bind the light uniform of the shader to the corresponding binding point of this light manager (for the time behing, always 0)
	//! This can be done once after the shader linkage
	void bindShader(Shader* shader) const;

	//! sent the lights data in the GPU
	void loadLight() const;

	//! sent the ambiant data in the GPU
	void loadAmbiant(Shader* shader) const;
};