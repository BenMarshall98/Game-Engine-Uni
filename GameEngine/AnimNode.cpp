#include "AnimNode.h"

AnimNode::~AnimNode()
{
	for (int i = 0; i < PositionKeys.size(); i++)
	{
		delete PositionKeys.at(i);
		PositionKeys.at(i) = nullptr;
	}

	PositionKeys.clear();

	for (int i = 0; i < RotationKeys.size(); i++)
	{
		delete RotationKeys.at(i);
		RotationKeys.at(i) = nullptr;
	}

	RotationKeys.clear();

	for (int i = 0; i < ScaleKeys.size(); i++)
	{
		delete ScaleKeys.at(i);
		ScaleKeys.at(i) = nullptr;
	}

	ScaleKeys.clear();
}