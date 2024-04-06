#pragma once

#include <glad/glad.h>
#include <string>
#include "../../main.h"


#ifdef IMGUI
struct imGuiTextureAttr {
	const GLuint textureID;
	int texture_unit;

	imGuiTextureAttr(const GLuint textureID, int texture_unit) : textureID(textureID) , texture_unit(texture_unit) {}
};

class Texture : public ImGuiPrintable
#else
class Texture
#endif
{
private:
	GLuint textureID;
	int texture_unit;
public:
	Texture(const char* file, int texture_unit=0);

	~Texture();

	void bind() const;

	void setTextureUnit(int texture_unit_) { texture_unit = texture_unit_; }

	int getTextureUnit() const { return texture_unit; }

	GLuint getID() const { return textureID; }

	void setID(GLuint _textureID) {textureID = _textureID; }

#ifdef IMGUI
	virtual void* getAttribute() const;

	virtual void updateAttribute(void* attr) const;

	virtual void setAttribute(void* attr);

	virtual void imGuiPrintAttribute(void* attr) const;
#endif
};