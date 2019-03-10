#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <string>

class Node
{
private:
	glm::mat4 transformation;
	std::string name;
	Node * parent;
	

public:
	Node(std::string pName, glm::mat4 pTransformation) :
		name(pName), transformation(pTransformation), parent(nullptr) {}
	~Node() {}

	void SetParent(Node * const pParent)
	{
		parent = pParent;
	}

	inline std::string GetName()
	{
		return name;
	}

	inline Node * GetParent()
	{
		return parent;
	}

	inline glm::mat4 GetTransform()
	{
		return transformation;
	}

	inline void SetTransform(const glm::mat4 pTransform)
	{
		transformation = pTransform;
	}
};