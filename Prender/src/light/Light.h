#pragma once

#include "../../main.h"
#include "../algebra/Algebra.h"
#include <glad/glad.h>
#include <vector>
#include "../../Shader.h"

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

	lightCaster(const vec3& lightColor, float falloff, const vec3& position, float cos_angle_min, const vec3& direction, float cos_angle_max) : lightColor(lightColor), falloff(falloff), position(position), cos_angle_min(cos_angle_min), direction(direction), cos_angle_max(cos_angle_max) {}
	lightCaster() : lightColor(), falloff(-1.0f), position(), cos_angle_min(), direction(), cos_angle_max() {}

};


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

	//! default light constructor
	Light(const vec3& lightColor, float falloff, const vec3& position, float cos_angle_min, const vec3& direction, float cos_angle_max);

	virtual ~Light();

	//! return the lighCaster to load in OpenGL
	virtual lightCaster getLightCaster() const;
};

class LightManager
{
private:
	const int MAX_LIGHT = 32;
	int lightBindingPoint;
	GLuint lightCatserUniformBuffer;
	std::vector<Light*> lights;
	std::vector<lightCaster> lightCasters;
	vec4 ambiantColor;
public:

	LightManager(const vec4& ambiantColor=vec4());

	~LightManager();

	void addLight(Light* light);

	void removeLight(Light* light);

	std::vector<Light*> getLights();

	void setAmbiantColor(const vec4& color);

	void computeLightCasters();

	std::vector<lightCaster> getLightCasters() const;

	//! bind the light uniform of the shader to the corresponding binding point of this light manager (for the time behing, always 0)
	void bindShader(Shader* shader) const;

	void loadLight() const;

	void loadAmbiant(Shader* shader) const;

};