#pragma once

#include "Texture.h"

class OpenGL330Texture : public Texture
{
private:
	unsigned int mTextureID;

public:
	OpenGL330Texture();
	~OpenGL330Texture();

	bool LoadTexture(const std::string & fileName) override;
};

