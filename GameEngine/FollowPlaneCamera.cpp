#include "FollowPlaneCamera.h"
#include "ComponentPosition.h"
#include "EntityManager.h"


FollowPlaneCamera::FollowPlaneCamera(Entity * const pEntity, const float pDistanceMin, const float pDistanceMax, const float pDistanceCurrent, const float pInterpolateRate) :
	Camera(nullptr), mEntity(pEntity), mDistanceMin(pDistanceMin), mDistanceMax(pDistanceMax), mDistanceCurrent(pDistanceCurrent), mInterpolateRate(pInterpolateRate)
{
}


FollowPlaneCamera::~FollowPlaneCamera()
{
}

void FollowPlaneCamera::Update()
{
	iComponent * const componentPosition = EntityManager::Instance()->GetComponentOfEntity(mEntity, ComponentType::COMPONENT_POSITION);

	const glm::vec3 playerPosition = dynamic_cast<ComponentPosition *>(componentPosition)->GetUpdatePosition();

	glm::vec3 cameraPosition = glm::vec3(playerPosition.x, playerPosition.y, playerPosition.z + mDistanceCurrent);
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
