#include "AIStateMachine.h"
#include "ScriptingManager.h"

AIStateMachine::~AIStateMachine()
{
}

//Gets the required value by valuename (read variable), creating the value and valuename if it doesn't exist
std::string AIStateMachine::GetValue(std::string & valueName, std::string & defaultValue)
{
	const std::map<std::string, std::string>::iterator it = stateValues.find(valueName);

	if (it != stateValues.end())
	{
		return it->second;
	}
	else
	{
		stateValues.insert(std::pair<std::string, std::string>(valueName, defaultValue));
		return defaultValue;
	}
}

//Sets valuename to the required value, creating the value and valuename if it doesn't exist
void AIStateMachine::SetValue(std::string & valueName, std::string & value)
{
	const std::map<std::string, std::string>::iterator it = stateValues.find(valueName);

	if (it != stateValues.end())
	{
		it->second = value;
	}
	else
	{
		stateValues.insert(std::pair<std::string, std::string>(valueName, value));
	}
}

//Moves the path follower off its path
void AIStateMachine::MoveOffPath()
{
	pathFollower->MoveOffPath();
}

//Finds a path given a location
void AIStateMachine::FindAIPath(const glm::vec3 & position, const glm::quat & direction, ComponentPhysics * const physicsComponent, const glm::vec3 & target, const float deltaTime)
{
	pathFinder->CalculatePathToPosition(position, direction, physicsComponent, target);
}

//Finds a path to the target
void AIStateMachine::FindAIPath(const glm::vec3 & position, const glm::quat & direction, ComponentPhysics * const physicsComponent, const float deltaTime)
{
	pathFinder->CalculatePath(position, direction, physicsComponent);
}

//Follows its path
void AIStateMachine::FindPath(const glm::vec3 & position, const glm::quat & direction, ComponentPhysics * const physicsComponent, const float deltaTime)
{
	pathFollower->CalculatePath(position, direction, physicsComponent);
}

//Gets the nearest pathnode to the Entity
glm::vec3 AIStateMachine::GetNearestPath(const glm::vec3 & position)
{
	return pathFollower->GetNearestPath(position);
}

//Calls the Lua function
void AIStateMachine::ResolveStates(Entity * const thisEntity, Entity * const target, float timePeriod)
{
	ScriptingManager::Instance()->RunScriptForStateAI(startFunction, thisEntity, target, this, timePeriod);
}