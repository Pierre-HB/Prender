#pragma once

#include <glad/glad.h>
#include <string>
#include "../../main.h"


#ifdef IMGUI
//! data for the texture UI
struct imGuiTextureAttr {
	const GLuint textureID;
	int texture_unit;

	//! Constructor
	imGuiTextureAttr(const GLuint textureID, int texture_unit) : textureID(textureID) , texture_unit(texture_unit) {
#ifdef DEBUG
		debug::NB_ATTR++;
#endif
	}
	~imGuiTextureAttr() {
#ifdef DEBUG
		debug::NB_ATTR--;
#endif
	}
};

//! A class for loading Textures in the shaders
class Texture : public ImGuiPrintable
#else
//! A class for loading Textures in the shaders
class Texture
#endif
{
private:

	GLuint textureID; //id of the texture
	int texture_unit;//!texture unit of the texture

	//! write the texture data in the gpu, return if the operation was succesfull
	bool setData(unsigned char* data, int width, int height, int nbChanels);

public:
	//! constructor of the texture from a file
	Texture(const char* file, int texture_unit = 0);

	//! constructor from a 8bit per channel image
	Texture(unsigned char* data, int width, int height, int nbChannels, int texture_unit=0);

	~Texture();

	//! bind
	void bind() const;

	//! change the texture unit
	void setTextureUnit(int texture_unit_) { texture_unit = texture_unit_; }

	//! getter for the texture unit
	int getTextureUnit() const { return texture_unit; }

	//! getter for the texture ID
	GLuint getID() const { return textureID; }

	//! Set the textureID. BECAREFULL, THE TEXTURE ID IS NOT SUPPOSED TO CHANGE
	void setID(GLuint _textureID) {textureID = _textureID; }

	//! create a 16*16 image tilling woth all shade of grey
	static Texture* createDebugMaterial(int width, int height, int texture_unit);

#ifdef IMGUI
	//! \copydoc ImGuiPrintable::getAttribute()
	virtual void* getAttribute() const;

	//! \copydoc ImGuiPrintable::updateAttribute()
	virtual void updateAttribute(void* attr) const;

	//! \copydoc ImGuiPrintable::setAttribute()
	virtual void setAttribute(void* attr);

	//! \copydoc ImGuiPrintable::imGuiPrintAttribute()
	virtual void imGuiPrintAttribute(void* attr) const;

	//! \copydoc ImGuiPrintable::deleteAttribute()
	virtual void deleteAttribute(void* attr) const;
#endif
};