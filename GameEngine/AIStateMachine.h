#pragma once

#include <string>
#include "glm/glm.hpp"
#include "ComponentPhysics.h"

using namespace std;
using namespace glm;

class AIStateMachine
{
private:
	string startFunction;
public:
	explicit AIStateMachine(string pStartFunction) : startFunction(pStartFunction) {}
	~AIStateMachine();

	//void TraverseStates(vec3 currentPosition, quat currentDirection, ComponentPhysics * physicsComponent);
};

