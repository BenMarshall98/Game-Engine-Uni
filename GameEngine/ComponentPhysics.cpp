#include "ComponentPhysics.h"
#include "ScriptingManager.h"
#include <algorithm>

#define E(s) #s
vector<string> ComponentPhysics::EntityTypeNames = { ENTITYTYPES(E) };
#undef E

ComponentPhysics::~ComponentPhysics()
{

}

void ComponentPhysics::RenderSwap()
{

}

ComponentType ComponentPhysics::GetComponentName()
{
	return ComponentType::COMPONENT_PHYSICS;
}

void ComponentPhysics::AddCollision(Entity * physicsComponent, EntityType pEntityType)
{
	map<Entity *, EntityType>::iterator it;
	it = unresolvedCollisions.find(physicsComponent);

	if (it == unresolvedCollisions.end())
	{
		unresolvedCollisions.insert(pair<Entity *, EntityType>(physicsComponent, pEntityType));
	}
}

void ComponentPhysics::ResolveCollisions()
{
	map<Entity *, EntityType>::iterator it;

	const ScriptingManager * const scriptingManager = ScriptingManager::Instance();

	for (it = unresolvedCollisions.begin(); it != unresolvedCollisions.end(); ++it)
	{
		const EntityType pEntityType = it->second;

		map<EntityType, string>::iterator collision = collisionFunctions->find(pEntityType);
		
		if (collision != collisionFunctions->end())
		{
			const string function = collision->second;
			scriptingManager->RunScriptFromCollision(function, thisEntity);
		}
	}

	unresolvedCollisions.clear();
}

EntityType ComponentPhysics::StringToEnum(const string & entityName)
{
	for (int i = 0; i < EntityTypeNames.size(); i++)
	{
		if (EntityTypeNames.at(i) == entityName)
		{
			return static_cast<EntityType>(i);
		}
	}

	return EntityType::NONE;
}