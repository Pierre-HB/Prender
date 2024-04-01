#pragma once

#include "Light.h"

class Light_Constant_Point : public Light
{
public:
	Light_Constant_Point(const vec3& lightColor, const vec3& position);

	~Light_Constant_Point();

};