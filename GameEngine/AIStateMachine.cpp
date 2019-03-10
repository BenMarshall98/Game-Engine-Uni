#include "AIStateMachine.h"
#include "ScriptingManager.h"

AIStateMachine::~AIStateMachine()
{
}

std::string AIStateMachine::GetValue(std::string valueName, std::string defaultValue)
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

void AIStateMachine::SetValue(std::string valueName, std::string value)
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

void AIStateMachine::MoveOffPath()
{
	pathFollower->MoveOffPath();
}

void AIStateMachine::FindAIPath(const glm::vec3 position, const glm::quat direction, ComponentPhysics * const physicsComponent, const glm::vec3 target, const float deltaTime)
{
	pathFinder->CalculatePathToPosition(position, direction, physicsComponent, target);
}

void AIStateMachine::FindAIPath(const glm::vec3 position, const glm::quat direction, ComponentPhysics * const physicsComponent, const float deltaTime)
{
	pathFinder->CalculatePath(position, direction, physicsComponent);
}

void AIStateMachine::FindPath(const glm::vec3 position, const glm::quat direction, ComponentPhysics * const physicsComponent, const float deltaTime)
{
	pathFollower->CalculatePath(position, direction, physicsComponent);
}

glm::vec3 AIStateMachine::GetNearestPath(const glm::vec3 position)
{
	return pathFollower->GetNearestPath(position);
}

void AIStateMachine::ResolveStates(Entity * const thisEntity, Entity * const target)
{
	ScriptingManager::Instance()->RunScriptForStateAI(startFunction, thisEntity, target, this, (1 / 60.0));
}