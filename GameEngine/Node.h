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
	Node(std::string & pName, glm::mat4 & pTransformation) :
		name(pName), transformation(pTransformation), parent(nullptr) {}
	~Node() {}

	Node& operator=(const Node&) = delete;
	Node(Node&) = delete;

	//Sets the parent node
	void SetParent(Node * const pParent)
	{
		parent = pParent;
	}

	//Gets the node name
	inline std::string & GetName()
	{
		return name;
	}

	//Gets the parent node
	inline Node * GetParent()
	{
		return parent;
	}

	//Gets the matrix transform
	inline glm::mat4 & GetTransform()
	{
		return transformation;
	}

	//Sets the matrix transform
	inline void SetTransform(const glm::mat4 & pTransform)
	{
		transformation = pTransform;
	}
};