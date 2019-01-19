#pragma once
#include "OpenGL.h"
#include <string>

using namespace std;


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

	bool LoadTexture(const string & fileName);

	inline int TextureID() const
	{
		return mTextureID;
	}
};

