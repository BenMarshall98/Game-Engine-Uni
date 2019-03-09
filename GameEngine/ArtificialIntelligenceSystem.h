#pragma once

#include "iSystem.h"
#include "EntityManager.h"
#include "Entity.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "ComponentPhysics.h"
#include "ComponentArtificalIntelligence.h"
#include <vector>

class ArtificialIntelligenceSystem : public iSystem
{
private:
	std::vector<Entity *> EntityList;
	EntityManager * entityManager;
	const ComponentType componentTypes[4] = {
		ComponentType::COMPONENT_POSITION,
		ComponentType::COMPONENT_DIRECTION,
		ComponentType::COMPONENT_PHYSICS,
		ComponentType::COMPONENT_ARTIFICAL_INTELLIGENCE
	};

	static void Intelligence(ComponentArtificalIntelligence * artificalIntelligence, Entity * pEntity);
public:
	ArtificialIntelligenceSystem();
	~ArtificialIntelligenceSystem();

	ArtificialIntelligenceSystem& operator=(const ArtificialIntelligenceSystem&) = delete;
	ArtificialIntelligenceSystem(ArtificialIntelligenceSystem&) = delete;

	void Action(void) override;
	void RemoveEntity(Entity * pEntity) override;
	void AddEntity(Entity * pEntity) override;
};

