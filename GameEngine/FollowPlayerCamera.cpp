#include "FollowPlayerCamera.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"
#include "EntityManager.h"
#include "glm/gtc/quaternion.hpp"


FollowPlayerCamera::FollowPlayerCamera(Entity * pEntity, float pDistanceMin, float pDistanceMax, float pDistanceCurrent, float pInterpolateRate) :
	mEntity(pEntity), mDistanceMin(pDistanceMin), mDistanceMax(pDistanceMax), mDistanceCurrent(pDistanceCurrent), mInterpolateRate(pInterpolateRate)
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

	vec3 forwardAxis = vec3(1, 0, 0);
	vec3 newDirection = forwardAxis * inverse(playerRotation);
	newDirection = newDirection * mDistanceCurrent;
	vec3 cameraPosition = playerPosition + newDirection;
	cameraPosition.y += 2;
	vec3 upDirection = vec3(0, 1, 0);

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
