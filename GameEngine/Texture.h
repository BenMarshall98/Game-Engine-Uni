#pragma once
#include "OpenGL.h"
#include <string>

class Texture
{
private:
	unsigned int mTextureID;
	Texture& operator=(const Texture& texture) = delete;
	Texture(const Texture& texture) = delete;

public:
	Texture() : mTextureID(0)
	{

	}

	~Texture()
	{
		glDeleteTextures(1, &mTextureID);
	}

	bool LoadTexture(const std::string & fileName);

	inline int TextureID() const
	{
		return mTextureID;
	}
};

