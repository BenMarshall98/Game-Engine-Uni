#include "FollowPlaneCamera.h"
#include "ComponentPosition.h"
#include "EntityManager.h"


FollowPlaneCamera::FollowPlaneCamera(Entity * pEntity, Plane pPlane, float pDistanceMin, float pDistanceMax, float pDistanceCurrent, float pInterpolateRate) :
	mEntity(pEntity), mPlane(pPlane), mDistanceMin(pDistanceMin), mDistanceMax(pDistanceMax), mDistanceCurrent(pDistanceCurrent), mInterpolateRate(pInterpolateRate)
{
	Update();
}


FollowPlaneCamera::~FollowPlaneCamera()
{
}

void FollowPlaneCamera::Update()
{
	iComponent * componentPosition = EntityManager::Instance()->GetComponentOfEntity(mEntity, COMPONENT_POSITION);

	vec3 playerPosition = dynamic_cast<ComponentPosition *>(componentPosition)->GetPosition();

	vec3 cameraPosition;
	vec3 upDirection;

	switch (mPlane)
	{
		case XY:
			cameraPosition = vec3(playerPosition.x, playerPosition.y, playerPosition.z + mDistanceCurrent);
			upDirection = vec3(0, 1, 0);
			break;
		case XZ:
			cameraPosition = vec3(playerPosition.x, playerPosition.y + mDistanceCurrent, playerPosition.z);
			upDirection = vec3(0, 0, 1);
			break;
		case ZY:
			cameraPosition = vec3(playerPosition.x + mDistanceCurrent, playerPosition.y, playerPosition.z);
			upDirection = vec3(0, 0, 1);
			break;
		default:
			cameraPosition = playerPosition;
			upDirection = vec3(0, 1, 0);
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
