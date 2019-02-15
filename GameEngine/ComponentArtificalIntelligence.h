#pragma once
#include "PathFollowing.h"
#include "ComponentPhysics.h"

class ComponentArtificalIntelligence : public iComponent
{
private:
	PathFollowing * pathFollower;

public:
	ComponentArtificalIntelligence(PathFollowing * pPathFollower) : pathFollower(pPathFollower) {}
	~ComponentArtificalIntelligence();

	ComponentArtificalIntelligence& operator=(const ComponentArtificalIntelligence&) = delete;
	ComponentArtificalIntelligence(ComponentArtificalIntelligence&) = delete;

	ComponentType GetComponentName() override;
	void RenderSwap() override;

	inline void ResolveAI(vec3 currentPosition, quat currentDirection, ComponentPhysics * componentPhysics)
	{
		pathFollower->CalculatePath(currentPosition, currentDirection, componentPhysics);
	}
};

