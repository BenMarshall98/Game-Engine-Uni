#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"
#include "stb_image.h"

bool Texture::LoadTexture(const string & fileName) const
{

	//The following code is based on: https://learnopengl.com/Getting-started/Textures
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, numChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char * image = stbi_load(fileName.c_str(), &width, &height, &numChannels, 3);
	bool success = image;

	if (success)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	return success;
}
