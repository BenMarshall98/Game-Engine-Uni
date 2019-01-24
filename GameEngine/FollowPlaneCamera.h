#pragma once
#include "Camera.h"
#include "Entity.h"

class FollowPlaneCamera : public Camera
{
private:
	Entity * mEntity;
	float mDistanceMin;
	float mDistanceMax;
	float mDistanceCurrent;
	float mInterpolateRate;
	vec3 lastPosition;

public:
	FollowPlaneCamera(Entity * pEntity, float pDistanceMin, float pDistanceMax, float pDistanceCurrent, float pInterpolateRate);
	~FollowPlaneCamera();

	FollowPlaneCamera& operator=(const FollowPlaneCamera&) = delete;
	FollowPlaneCamera(FollowPlaneCamera&) = delete;

	void Update() override;
};

