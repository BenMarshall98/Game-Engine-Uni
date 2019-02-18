#pragma once

#include <string>
#include "glm/glm.hpp"
#include "ComponentPhysics.h"
#include <string>
#include <map>

using namespace std;
using namespace glm;

class AIStateMachine
{
private:
	map<string, string> stateValues = map<string, string>();
	string startFunction;
public:
	explicit AIStateMachine(string pStartFunction) : startFunction(pStartFunction) {}
	~AIStateMachine();

	//void TraverseStates(vec3 currentPosition, quat currentDirection, ComponentPhysics * physicsComponent);
	string GetValue(string valueName, string defaultValue);
};

