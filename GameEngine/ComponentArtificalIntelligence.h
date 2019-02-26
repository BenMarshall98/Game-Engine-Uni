#pragma once
#include "PathFollowing.h"
#include "PathFinding.h"
#include "AIStateMachine.h"
#include "Entity.h"
#include "ComponentPhysics.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"

class ComponentArtificalIntelligence : public iComponent
{
private:
	PathFollowing * pathFollower;
	PathFinding * pathFinding;
	AIStateMachine * aiStateMachine;
	Entity * target;

public:
	explicit ComponentArtificalIntelligence(PathFollowing * const pPathFollower, PathFinding * const pPathFinding, AIStateMachine * const pAIStateMachine, const string & pTarget) : pathFollower(pPathFollower), pathFinding(pPathFinding),
		aiStateMachine(pAIStateMachine), target(EntityManager::Instance()->GetEntityByName(pTarget))
	{
		if (aiStateMachine)
		{
			aiStateMachine->SetPathFinder(pathFinding);
			aiStateMachine->SetPathFollower(pathFollower);
		}
	}
	~ComponentArtificalIntelligence();

	ComponentArtificalIntelligence& operator=(const ComponentArtificalIntelligence&) = delete;
	ComponentArtificalIntelligence(ComponentArtificalIntelligence&) = delete;

	ComponentType GetComponentName() override;
	void RenderSwap() override;

	inline void ResolveAI(Entity * thisEntity)
	{
		if (aiStateMachine)
		{
			aiStateMachine->ResolveStates(thisEntity, target);
		}
		else
		{
			iComponent * componentPosition = EntityManager::Instance()->GetComponentOfEntity(thisEntity, ComponentType::COMPONENT_POSITION);
			iComponent * componentDirection = EntityManager::Instance()->GetComponentOfEntity(thisEntity, ComponentType::COMPONENT_DIRECTION);
			iComponent * componentPhysics = EntityManager::Instance()->GetComponentOfEntity(thisEntity, ComponentType::COMPONENT_PHYSICS);

			vec3 position = dynamic_cast<ComponentPosition *>(componentPosition)->GetRenderPosition();
			quat direction = dynamic_cast<ComponentDirection *>(componentDirection)->GetRenderDirection();
			ComponentPhysics * physics = dynamic_cast<ComponentPhysics *>(componentPhysics);

			if (pathFinding)
			{
				pathFinding->CalculatePath(position, direction, physics);
			}
			else if (pathFollower)
			{
				pathFollower->CalculatePath(position, direction, physics);
			}
		}
		

		
	}
};

