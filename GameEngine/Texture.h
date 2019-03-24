#pragma once
#include "OpenGL.h"
#include <string>

class Texture
{
private:
public:
	Texture();

	~Texture();

	Texture& operator=(const Texture& texture) = delete;
	Texture(const Texture& texture) = delete;

	virtual bool LoadTexture(const std::string & fileName) = 0;
	virtual void BindTexture() = 0;
};

