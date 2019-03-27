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
	static void Intelligence(ComponentArtificalIntelligence * artificalIntelligence, Entity * pEntity, float timePeriod);
public:
	ArtificialIntelligenceSystem();
	~ArtificialIntelligenceSystem();

	ArtificialIntelligenceSystem& operator=(const ArtificialIntelligenceSystem&) = delete;
	ArtificialIntelligenceSystem(ArtificialIntelligenceSystem&) = delete;

	void Action(float timePeriod) override;
};

