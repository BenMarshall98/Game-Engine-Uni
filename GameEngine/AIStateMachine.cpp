#include "AIStateMachine.h"
#include "ScriptingManager.h"

AIStateMachine::~AIStateMachine()
{
}

string AIStateMachine::GetValue(string valueName, string defaultValue)
{
	map<string, string>::iterator it = stateValues.find(valueName);

	if (it != stateValues.end())
	{
		return it->second;
	}
	else
	{
		stateValues.insert(pair<string, string>(valueName, defaultValue));
		return defaultValue;
	}
}

void AIStateMachine::SetValue(string valueName, string value)
{
	map<string, string>::iterator it = stateValues.find(valueName);

	if (it != stateValues.end())
	{
		it->second = value;
	}
	else
	{
		stateValues.insert(pair<string, string>(valueName, value));
	}
}

void AIStateMachine::MoveOffPath()
{
	pathFollower->MoveOffPath();
}

void AIStateMachine::FindAIPath(vec3 position, quat direction, ComponentPhysics * physicsComponent, vec3 target, float deltaTime)
{
	pathFinder->CalculatePathToPosition(position, direction, physicsComponent, target);
}

void AIStateMachine::FindAIPath(vec3 position, quat direction, ComponentPhysics * physicsComponent, float deltaTime)
{
	pathFinder->CalculatePath(position, direction, physicsComponent);
}

void AIStateMachine::FindPath(vec3 position, quat direction, ComponentPhysics * physicsComponent, float deltaTime)
{
	pathFollower->CalculatePath(position, direction, physicsComponent);
}

vec3 AIStateMachine::GetNearestPath(vec3 position)
{
	return pathFollower->GetNearestPath(position);
}

void AIStateMachine::ResolveStates(Entity * thisEntity, Entity * target)
{
	ScriptingManager::Instance()->RunScriptForStateAI(startFunction, thisEntity, target, this, (1 / 60.0));
}