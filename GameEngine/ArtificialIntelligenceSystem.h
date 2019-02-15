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
	vector<Entity *> EntityList;
	EntityManager * entityManager;

	static void Intelligence(vec3 position, quat direction, ComponentArtificalIntelligence * artificalIntelligence, ComponentPhysics * physics);
public:
	ArtificialIntelligenceSystem();
	~ArtificialIntelligenceSystem();

	ArtificialIntelligenceSystem& operator=(const ArtificialIntelligenceSystem&) = delete;
	ArtificialIntelligenceSystem(ArtificialIntelligenceSystem&) = delete;

	void Action(void) override;
	void RemoveEntity(Entity * pEntity) override;
};

