#include "FollowPlaneCamera.h"
#include "ComponentPosition.h"
#include "EntityManager.h"


FollowPlaneCamera::FollowPlaneCamera(Entity * pEntity, float pDistanceMin, float pDistanceMax, float pDistanceCurrent, float pInterpolateRate) :
	Camera(nullptr), mEntity(pEntity), mDistanceMin(pDistanceMin), mDistanceMax(pDistanceMax), mDistanceCurrent(pDistanceCurrent), mInterpolateRate(pInterpolateRate)
{
}


FollowPlaneCamera::~FollowPlaneCamera()
{
}

void FollowPlaneCamera::Update()
{
	iComponent * componentPosition = EntityManager::Instance()->GetComponentOfEntity(mEntity, ComponentType::COMPONENT_POSITION);

	vec3 playerPosition = dynamic_cast<ComponentPosition *>(componentPosition)->GetUpdatePosition();

	vec3 cameraPosition = vec3(playerPosition.x, playerPosition.y, playerPosition.z + mDistanceCurrent);
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

	SetLookAt(direction);
	SetPosition(cameraPosition);
	SetUp(upDirection);

	lastPosition = cameraPosition;

	Camera::Update();
}
