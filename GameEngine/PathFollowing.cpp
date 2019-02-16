#include "PathFollowing.h"
#include "glm/gtx/matrix_transform_2d.hpp"

PathFollowing::~PathFollowing()
{
}

void PathFollowing::CalculatePath(const vec3 & currentPosition, const quat & currentDirection, ComponentPhysics * const physicsComponent)
{
	const float disVelocity = 200 * (1.0 / 60.0);
	const float disRotation = 200 * (1.0 / 60.0);

	if (currentNode == -1)
	{
		int closestNode = 0;
		float distance = length(pathNodes.at(0)->position - currentPosition);

		for (int i = 1; i < pathNodes.size(); i++)
		{
			const float newDistance = length(pathNodes.at(i)->position - currentPosition);

			if (newDistance < distance)
			{
				distance = newDistance;
				closestNode = i;
			}
		}

		currentNode = closestNode;
	}

	const float distanceToNode = length(pathNodes.at(currentNode)->position - currentPosition);

	if (distanceToNode < pathNodes.at(currentNode)->radius)
	{
		currentNode++;

		if (currentNode == pathNodes.size())
		{
			currentNode = 0;
		}
	}

	const vec3 norm = normalize(pathNodes.at(currentNode)->position - currentPosition);

	//mat3 matRotation = toMat3(currentDirection);
	//vec3 velocity = matRotation * vec3(1, 0, 0);
	//
	//float angle = atan2(
	//	velocity.x * norm.z - velocity.z * norm.x,
	//	velocity.x * norm.x + velocity.z * norm.z);

	////angle = angle * 0.1;

	//angle *= disRotation;

	//vec3 rotation = vec3(0.0, angle, 0.0);

	vec3 velocity = norm;

	velocity *= disVelocity;

	physicsComponent->SetUpdateVelocity(velocity);
	//physicsComponent->SetUpdateRotation(rotation);
}
