#include "PathFollowing.h"
#include "glm/gtx/matrix_transform_2d.hpp"

PathFollowing::~PathFollowing()
{
}

void PathFollowing::CalculatePath(const glm::vec3 & currentPosition, const glm::quat & currentDirection, ComponentPhysics * const physicsComponent)
{
	const float disVelocity = 3.5;

	if (currentNode == -1)
	{
		int closestNode = 0;
		float distance = glm::length(pathNodes->at(0)->position - currentPosition);

		for (int i = 1; i < pathNodes->size(); i++)
		{
			const float newDistance = length(pathNodes->at(i)->position - currentPosition);

			if (newDistance < distance)
			{
				distance = newDistance;
				closestNode = i;
			}
		}

		currentNode = closestNode;
	}

	const float distanceToNode = length(pathNodes->at(currentNode)->position - currentPosition);

	if (distanceToNode < pathNodes->at(currentNode)->radius)
	{
		currentNode++;

		if (currentNode == pathNodes->size())
		{
			currentNode = 0;
		}
	}

	const glm::vec3 norm = normalize(pathNodes->at(currentNode)->position - currentPosition);

	//mat3 matRotation = toMat3(currentDirection);
	//vec3 velocity = matRotation * vec3(1, 0, 0);
	//
	//float angle = atan2(
	//	velocity.x * norm.z - velocity.z * norm.x,
	//	velocity.x * norm.x + velocity.z * norm.z);

	////angle = angle * 0.1;

	//angle *= disRotation;

	//vec3 rotation = vec3(0.0, angle, 0.0);

	glm::vec3 velocity = norm;

	velocity *= disVelocity;

	physicsComponent->SetUpdateVelocity(velocity);
	//physicsComponent->SetUpdateRotation(rotation);
}

bool PathFollowing::OnPath(const glm::vec3 & position)
{
	if (onPath)
	{
		return true;
	}

	for (int i = 0; i < pathNodes->size(); i++)
	{
		if (glm::length(position - pathNodes->at(i)->position) <= pathNodes->at(i)->radius)
		{
			onPath = true;
			currentNode = i;
			return true;
		}
	}

	return false;
}

glm::vec3 PathFollowing::GetNearestPath(const glm::vec3 & position)
{
	int closestNode = 0;
	float distance = glm::length(pathNodes->at(0)->position - position);

	for (int i = 1; i < pathNodes->size(); i++)
	{
		const float newDistance = length(pathNodes->at(i)->position - position);

		if (newDistance < distance)
		{
			distance = newDistance;
			closestNode = i;
		}
	}

	return pathNodes->at(closestNode)->position;
}
