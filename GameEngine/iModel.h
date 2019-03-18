#pragma once
#include "Shader.h"
#include <vector>
#include "glm/glm.hpp""

class iModel
{
private:
	
public:
	iModel() {};
	virtual ~iModel();
	virtual void Render(Shader * shader) = 0;
	virtual void Render(Shader * shader, std::vector<glm::mat4> boneMats) = 0;
};
