#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <string>

using namespace std;
using namespace glm;

class Node
{
private:
	string name;
	Node * parent;
	mat4 transformation;

public:
	Node(string pName, mat4 pTransformation) :
		name(pName), transformation(pTransformation), parent(nullptr) {}
	~Node() {}

	void SetParent(Node * pParent)
	{
		parent = pParent;
	}

	inline string GetName()
	{
		return name;
	}

	inline Node * GetParent()
	{
		return parent;
	}

	inline mat4 GetTransform()
	{
		return transformation;
	}

	inline void SetTransform(mat4 pTransform)
	{
		transformation = pTransform;
	}
};