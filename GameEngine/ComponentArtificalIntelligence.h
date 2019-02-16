#pragma once
#include "PathFollowing.h"
#include "PathFinding.h"
#include "ComponentPhysics.h"

class ComponentArtificalIntelligence : public iComponent
{
private:
	PathFollowing * pathFollower;
	PathFinding * pathFinding;

public:
	explicit ComponentArtificalIntelligence(PathFollowing * const pPathFollower, PathFinding * const pPathFinding) : pathFollower(pPathFollower), pathFinding(pPathFinding) {}
	~ComponentArtificalIntelligence();

	ComponentArtificalIntelligence& operator=(const ComponentArtificalIntelligence&) = delete;
	ComponentArtificalIntelligence(ComponentArtificalIntelligence&) = delete;

	ComponentType GetComponentName() override;
	void RenderSwap() override;

	inline void ResolveAI(const vec3 & currentPosition, const quat & currentDirection, ComponentPhysics * const componentPhysics)
	{
		if (pathFollower)
		{
			pathFollower->CalculatePath(currentPosition, currentDirection, componentPhysics);
		}

		if (pathFinding)
		{
			pathFinding->CalculatePath(currentPosition, currentDirection, componentPhysics);
		}
	}
};

