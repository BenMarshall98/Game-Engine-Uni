#pragma once
#include "Camera.h"
#include "Entity.h"


class FollowPlayerCamera : public Camera
{
private:
	vec3 lastPosition;
	Entity * mEntity;
	float mDistanceMin;
	float mDistanceMax;
	float mDistanceCurrent;
	float mInterpolateRate;
	

public:
	FollowPlayerCamera(Entity * pEntity, float pDistanceMin, float pDistanceMax, float pDistanceCurrent, float pInterploateRate);
	~FollowPlayerCamera();

	FollowPlayerCamera& operator=(const FollowPlayerCamera&) = delete;
	FollowPlayerCamera(FollowPlayerCamera&) = delete;

	void Update() override;
};

