#include "texture.h"

//image loader
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::Texture(const char* file, int texture_unit) : textureID(), texture_unit(texture_unit) {

	//read the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data)
	{
		glGenTextures(1, &textureID);

		bind();
		//set border operations
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//load data
		switch (nrChannels)
		{
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
			break;
		case 2:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);


			break;
		default:
#ifdef CONSOLE
			std::cout << "[] Failed to send texture " << file << " on GPU: Uknown number of channel : " << nrChannels << " must be either 1, 2, 3 or 4." << std::endl;
#endif
			break;
		}

		//compute the mipmap, may not be alwayse usefull
		glGenerateMipmap(GL_TEXTURE_2D);
	}
#ifdef CONSOLE
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
#endif

	stbi_image_free(data);

#ifdef IMGUI
	ImGuiManager::addObject(ImGuiObjectType::UTILS_TEXTURE, this);
#endif
}

Texture::~Texture() {
	glDeleteTextures(1, &textureID);
#ifdef IMGUI
	ImGuiManager::removeObject(ImGuiObjectType::UTILS_TEXTURE, this);
#endif
}

void Texture::bind() const {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}


#ifdef IMGUI
void* Texture::getAttribute() const {
	return new imGuiTextureAttr(textureID, texture_unit);
}

void Texture::updateAttribute(void* attr) const {
	static_cast<imGuiTextureAttr*>(attr)->texture_unit = texture_unit;
}

void Texture::setAttribute(void* attr) {
	texture_unit = static_cast<imGuiTextureAttr*>(attr)->texture_unit;
}

void Texture::imGuiPrintAttribute(void* attr) const {
	ImGui::Text("TextureID : %u", static_cast<imGuiTextureAttr*>(attr)->textureID);

	ImGui::InputInt("Texture unit", &static_cast<imGuiTextureAttr*>(attr)->texture_unit);

	ImGui::Image((void*)(intptr_t)static_cast<imGuiTextureAttr*>(attr)->textureID, ImVec2(64, 64));
}
#endif