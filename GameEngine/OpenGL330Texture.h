#pragma once

#include "Texture.h"

class OpenGL330Texture : public Texture
{
private:
	unsigned int mTextureID;

public:
	OpenGL330Texture();
	~OpenGL330Texture();

	OpenGL330Texture(const OpenGL330Texture&) = delete;
	OpenGL330Texture& operator=(const OpenGL330Texture&) = delete;

	bool LoadTexture(const std::string & fileName) override;
	void BindTexture() override;
};

