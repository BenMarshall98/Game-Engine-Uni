#pragma once
#include "PathFollowing.h"
#include "ComponentPhysics.h"

class ComponentArtificalIntelligence : public iComponent
{
private:
	PathFollowing * pathFollower;

public:
	explicit ComponentArtificalIntelligence(PathFollowing * const pPathFollower) : pathFollower(pPathFollower) {}
	~ComponentArtificalIntelligence();

	ComponentArtificalIntelligence& operator=(const ComponentArtificalIntelligence&) = delete;
	ComponentArtificalIntelligence(ComponentArtificalIntelligence&) = delete;

	ComponentType GetComponentName() override;
	void RenderSwap() override;

	inline void ResolveAI(const vec3 & currentPosition, const quat & currentDirection, ComponentPhysics * const componentPhysics)
	{
		pathFollower->CalculatePath(currentPosition, currentDirection, componentPhysics);
	}
};

