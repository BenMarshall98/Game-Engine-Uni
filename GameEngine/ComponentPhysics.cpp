#include "ComponentPhysics.h"
#include "ScriptingManager.h"
#include <algorithm>

#define E(s) #s
std::vector<std::string> ComponentPhysics::EntityTypeNames = { ENTITYTYPES(E) };
#undef E

ComponentPhysics::~ComponentPhysics()
{
	delete rigidBody;
	delete collisionFunctions;
	delete shape;
}

//Does nothing
void ComponentPhysics::RenderSwap()
{

}

//Adds collision ensuring only one of each collision is stored
void ComponentPhysics::AddCollision(Entity * physicsComponent, EntityType pEntityType)
{
	std::map<Entity *, EntityType>::iterator it;
	it = unresolvedCollisions.find(physicsComponent);

	if (it == unresolvedCollisions.end())
	{
		unresolvedCollisions.insert(std::pair<Entity *, EntityType>(physicsComponent, pEntityType));
	}
}

//Calls Lua function according to the Entity Type collided with
void ComponentPhysics::ResolveCollisions()
{
	std::map<Entity *, EntityType>::iterator it;

	const ScriptingManager * const scriptingManager = ScriptingManager::Instance();

	for (it = unresolvedCollisions.begin(); it != unresolvedCollisions.end(); ++it)
	{
		const EntityType pEntityType = it->second;

		std::map<EntityType, std::string>::iterator collision = collisionFunctions->find(pEntityType);
		
		if (collision != collisionFunctions->end())
		{
			const std::string function = collision->second;
			scriptingManager->RunScriptFromCollision(function, thisEntity);
		}
	}

	unresolvedCollisions.clear();
}

//Turns string to EntityType enum
EntityType ComponentPhysics::StringToEnum(const std::string & entityName)
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