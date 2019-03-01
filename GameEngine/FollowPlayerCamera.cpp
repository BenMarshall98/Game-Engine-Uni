#include "FollowPlayerCamera.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"
#include "EntityManager.h"
#include "glm/gtc/quaternion.hpp"


FollowPlayerCamera::FollowPlayerCamera(Entity * const pEntity, const float pDistanceMin, const float pDistanceMax, const float pDistanceCurrent, const float pInterpolateRate) :
	Camera(nullptr), mEntity(pEntity), mDistanceMin(pDistanceMin), mDistanceMax(pDistanceMax), mDistanceCurrent(pDistanceCurrent), mInterpolateRate(pInterpolateRate)
{
}


FollowPlayerCamera::~FollowPlayerCamera()
{
}

void FollowPlayerCamera::Update()
{
	iComponent * const componentPosition = EntityManager::Instance()->GetComponentOfEntity(mEntity, ComponentType::COMPONENT_POSITION);
	iComponent * const componentDirection = EntityManager::Instance()->GetComponentOfEntity(mEntity, ComponentType::COMPONENT_DIRECTION);

	const glm::vec3 playerPosition = dynamic_cast<ComponentPosition *>(componentPosition)->GetUpdatePosition();
	const glm::quat playerRotation = dynamic_cast<ComponentDirection *>(componentDirection)->GetUpdateDirection();

	const glm::vec3 forwardAxis = glm::vec3(1, 0, 0);
	glm::vec3 newDirection = forwardAxis * inverse(playerRotation);
	newDirection = newDirection * mDistanceCurrent;
	glm::vec3 cameraPosition = playerPosition + newDirection;
	cameraPosition.y += 2;
	const glm::vec3 upDirection = glm::vec3(0, 1, 0);

	static bool firstTime = true;

	if (!firstTime)
	{
		cameraPosition = glm::mix(lastPosition, cameraPosition, mInterpolateRate);
	}
	else
	{
		firstTime = !firstTime;
	}

	const glm::vec3 direction = normalize(playerPosition - cameraPosition);

	SetLookAt(direction);
	SetPosition(cameraPosition);
	SetUp(upDirection);

	lastPosition = cameraPosition;

	Camera::Update();
}
