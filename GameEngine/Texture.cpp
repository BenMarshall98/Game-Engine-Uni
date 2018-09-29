#include "Texture.h"

#include "IL/il.h"

bool Texture::LoadTexture(string fileName)
{

	//The following code is based on: https://r3dux.org/tag/ilutglloadimage/
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned int width, height, imageID;
	bool success;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	success = ilLoadImage(fileName.c_str());

	if (success)
	{
		ILinfo ImageInfo
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}


	glBindTexture(GL_TEXTURE_2D, 0);

	return image;
}
