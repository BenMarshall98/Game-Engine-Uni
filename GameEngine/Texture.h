#pragma once
#include "OpenGL.h"
#include <string>

using namespace std;


class Texture
{
private:
	unsigned int mTextureID;

public:
	Texture() : mTextureID(0)
	{

	}

	~Texture()
	{
		glDeleteTextures(1, &mTextureID);
	}

	bool LoadTexture(string fileName);
};

