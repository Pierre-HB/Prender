#include "texture.h"
//image loader
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "../../main.h"


bool Texture::setData(unsigned char* data, int width, int height, int nbChanels) {
	glGenTextures(1, &textureID);
#ifdef DEBUG
	debug::NB_OPENGL_PTR++;
#endif
	bind();
	//set border operations
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//load data
	switch (nbChanels)
	{
	case 1:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		break;
	case 2:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
		break;
	case 3:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		break;
	case 4:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);


		break;
	default:
		return false;
	}

	//compute the mipmap, may not be alwayse usefull
	glGenerateMipmap(GL_TEXTURE_2D);
	return true;
}

Texture::Texture(const char* file, int texture_unit) : textureID(), texture_unit(texture_unit) {

	//read the texture
	int width, height, nbChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(file, &width, &height, &nbChannels, 0);
	if (data)
	{
		if (!setData(data, width, height, nbChannels)) {
#ifdef CONSOLE
			std::cout << "[] Failed to send texture " << file << " on GPU: Uknown number of channel : " << nbChannels << " must be either 1, 2, 3 or 4." << std::endl;
#endif
		}
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
#ifdef DEBUG
	debug::NB_INSTANCES++;
#endif
}

Texture::Texture(unsigned char* data, int width, int height, int nbChannels, int texture_unit) : textureID(), texture_unit(texture_unit) {
	if (data)
	{
		if (!setData(data, width, height, nbChannels)) {
#ifdef CONSOLE
			std::cout << "[] Failed to send texture engine-made on GPU: Uknown number of channel : " << nbChannels << " must be either 1, 2, 3 or 4." << std::endl;
#endif
		}
	}
#ifdef CONSOLE
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
#endif

#ifdef IMGUI
	ImGuiManager::addObject(ImGuiObjectType::UTILS_TEXTURE, this);
#endif
#ifdef DEBUG
	debug::NB_INSTANCES++;
#endif
}


Texture::~Texture() {
	glDeleteTextures(1, &textureID);
#ifdef IMGUI
	ImGuiManager::removeObject(ImGuiObjectType::UTILS_TEXTURE, this);
#endif
#ifdef DEBUG
	debug::NB_INSTANCES--;
	debug::NB_OPENGL_PTR--;
#endif
}

void Texture::bind() const {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

Texture* Texture::createDebugMaterial(int width, int height, int texture_unit) {
	std::vector<unsigned char> data = std::vector<unsigned char>(width * height * 2, 0);

	for (int i = 0; i < width * height; i++) {
		int x = i / height; // line
		int y = i % height; // collumn
		int _x = x * 16 / width;
		int _y = y * 16 / height;
		unsigned char roughness = 16 * _x + _y;
		data[2 * i] = roughness;
		data[2 * i + 1] = roughness;
		//data[3 * i + 2] = roughness;
	}
	return new Texture(data.data(), width, height, 2, texture_unit);
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

	//ImGui::Image((void*)(intptr_t)static_cast<imGuiTextureAttr*>(attr)->textureID, ImVec2(64, 64));
	ImGui::Image((void*)(intptr_t)static_cast<imGuiTextureAttr*>(attr)->textureID, ImVec2(128, 128));
}

void Texture::deleteAttribute(void* attr) const {
	delete static_cast<imGuiTextureAttr*>(attr);
}
#endif