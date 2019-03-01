#include "AIStateMachine.h"
#include "ScriptingManager.h"

AIStateMachine::~AIStateMachine()
{
}

std::string AIStateMachine::GetValue(std::string valueName, std::string defaultValue)
{
	std::map<std::string, std::string>::iterator it = stateValues.find(valueName);

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

void AIStateMachine::SetValue(std::string valueName, std::string value)
{
	std::map<std::string, std::string>::iterator it = stateValues.find(valueName);

	if (it != stateValues.end())
	{
		it->second = value;
	}
	else
	{
		stateValues.insert(std::pair<std::string, std::string>(valueName, value));
	}
}

void AIStateMachine::MoveOffPath()
{
	pathFollower->MoveOffPath();
}

void AIStateMachine::FindAIPath(glm::vec3 position, glm::quat direction, ComponentPhysics * physicsComponent, glm::vec3 target, float deltaTime)
{
	pathFinder->CalculatePathToPosition(position, direction, physicsComponent, target);
}

void AIStateMachine::FindAIPath(glm::vec3 position, glm::quat direction, ComponentPhysics * physicsComponent, float deltaTime)
{
	pathFinder->CalculatePath(position, direction, physicsComponent);
}

void AIStateMachine::FindPath(glm::vec3 position, glm::quat direction, ComponentPhysics * physicsComponent, float deltaTime)
{
	pathFollower->CalculatePath(position, direction, physicsComponent);
}

glm::vec3 AIStateMachine::GetNearestPath(glm::vec3 position)
{
	return pathFollower->GetNearestPath(position);
}

void AIStateMachine::ResolveStates(Entity * thisEntity, Entity * target)
{
	ScriptingManager::Instance()->RunScriptForStateAI(startFunction, thisEntity, target, this, (1 / 60.0));
}