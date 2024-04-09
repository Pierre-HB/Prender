#pragma once

#include "../../Shader.h"

class Material
{
public:
	Material();

	~Material();

	virtual void setup(Shader* shader) = 0;
};