#pragma once
#include "Shader.h"

class iModel
{
private:
	
public:
	iModel() {};
	virtual ~iModel();
	virtual void Render(Shader * shader) = 0;
};
