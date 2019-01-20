#pragma once
#include "Camera.h"
#include "Entity.h"

enum LockPlane
{
	X,
	Y,
	Z
};

class FollowPlayerCamera : public Camera
{
private:
	Entity * mEntity;
	LockPlane mPlane;
	float mDistanceMin;
	float mDistanceMax;
	float mDistanceCurrent;
	float mInterpolateRate;
	vec3 lastPosition;

public:
	FollowPlayerCamera(Entity * pEntity, LockPlane pPlane, float pDistanceMin, float pDistanceMax, float pDistanceCurrent, float pInterploateRate);
	~FollowPlayerCamera();

	FollowPlayerCamera& operator=(const FollowPlayerCamera&) = delete;
	FollowPlayerCamera(FollowPlayerCamera&) = delete;

	void Update() override;
};

