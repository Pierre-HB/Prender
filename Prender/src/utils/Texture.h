#pragma once

#include <glad/glad.h>
#include <string>

class Texture
{
private:
	GLuint texture;
	int texture_unit;
public:
	Texture(const char* file, int texture_unit=0);

	~Texture();

	void bind() const;

	void setTextureUnit(int texture_unit_) { texture_unit = texture_unit_; }

	int getTextureUnit() { return texture_unit; }
};