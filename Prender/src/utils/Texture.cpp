#include "texture.h"

//image loader
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

//Texture::Texture() : texture(), texture_unit() {
//
//}

Texture::Texture(const char* file, int texture_unit) : texture(), texture_unit(texture_unit) {

	//read the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data)
	{
		glGenTextures(1, &texture);

		bind();
		//set border operations
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//bind();
		//glBindTexture(GL_TEXTURE_2D, texture);

		std::cout << nrChannels << std::endl;

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
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);

			std::cout << "loading 4 channeles" << std::endl;
			break;
		default:
			std::cout << "[] Failed to send texture " << file << " on GPU: Uknown number of channel : " << nrChannels << " must be either 1, 2, 3 or 4." << std::endl;
			break;
		}
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//compute the mipmap, may not be alwayse usefull
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
}

Texture::~Texture() {
	glDeleteTextures(1, &texture);
}

void Texture::bind() const {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, texture);
}


