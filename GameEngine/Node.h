#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <string>

using namespace std;
using namespace glm;

class Node
{
private:
	mat4 transformation;
	string name;
	Node * parent;
	

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