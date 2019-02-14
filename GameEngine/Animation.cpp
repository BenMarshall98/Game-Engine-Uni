#include "Animation.h"

Animation::~Animation()
{
	for (int i = 0; i < animNodes.size(); i++)
	{
		delete animNodes.at(i);
		animNodes.at(i) = nullptr;
	}

	animNodes.clear();
}