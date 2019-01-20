#include "FollowPlayerCamera.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"
#include "EntityManager.h"
#include "glm/gtc/quaternion.hpp"


FollowPlayerCamera::FollowPlayerCamera(Entity * pEntity, LockPlane pPlane, float pDistanceMin, float pDistanceMax, float pDistanceCurrent, float pInterpolateRate) :
	mEntity(pEntity), mPlane(pPlane), mDistanceMin(pDistanceMin), mDistanceMax(pDistanceMax), mDistanceCurrent(pDistanceCurrent), mInterpolateRate(pInterpolateRate)
{
}


FollowPlayerCamera::~FollowPlayerCamera()
{
}

void FollowPlayerCamera::Update()
{
	iComponent * componentPosition = EntityManager::Instance()->GetComponentOfEntity(mEntity, ComponentType::COMPONENT_POSITION);
	iComponent * componentDirection = EntityManager::Instance()->GetComponentOfEntity(mEntity, ComponentType::COMPONENT_DIRECTION);

	vec3 playerPosition = dynamic_cast<ComponentPosition *>(componentPosition)->GetUpdatePosition();
	quat playerRotation = dynamic_cast<ComponentDirection *>(componentDirection)->GetUpdateDirection();

	vec3 cameraPosition;
	vec3 upDirection;

	vec3 playerAxis = axis(playerRotation);

	switch (mPlane)
	{
		case X:
			playerAxis = normalize(playerAxis * vec3(0, 1, 1));
			cameraPosition = vec3(playerPosition.x, playerPosition.y + (playerAxis.y * mDistanceCurrent), playerPosition.z + (playerAxis.z * mDistanceCurrent));
			upDirection = vec3(1, 0, 0);
			break;
		case Y:
			playerAxis = normalize(playerAxis * vec3(1, 0, 1));
			cameraPosition = vec3(playerPosition.x + (playerAxis.x * mDistanceCurrent), playerPosition.y, playerPosition.z + (playerAxis.z * mDistanceCurrent));
			upDirection = vec3(0, 1, 0);
			break;
		case Z:
			playerAxis = normalize(playerAxis * vec3(1, 1, 0));
			cameraPosition = vec3(playerPosition.x + (playerAxis.x * mDistanceCurrent), playerPosition.y + (playerAxis.y * mDistanceCurrent), playerPosition.z);
			upDirection = vec3(0, 0, 1);
			break;
	}

	static bool firstTime = true;

	if (!firstTime)
	{
		cameraPosition = mix(lastPosition, cameraPosition, mInterpolateRate);
	}
	else
	{
		firstTime = !firstTime;
	}

	vec3 direction = normalize(playerPosition - cameraPosition);

	lookAt = direction;
	position = cameraPosition;
	up = upDirection;

	lastPosition = cameraPosition;

	Camera::Update();
}
