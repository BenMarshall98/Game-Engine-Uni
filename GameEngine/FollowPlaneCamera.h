#pragma once
#include "Camera.h"
#include "Entity.h"

class FollowPlaneCamera : public Camera
{
private:
	glm::vec3 lastPosition;
	Entity * mEntity;
	float mDistanceMin;
	float mDistanceMax;
	float mDistanceCurrent;
	float mInterpolateRate;
	

public:
	FollowPlaneCamera(Entity * pEntity, float pDistanceMin, float pDistanceMax, float pDistanceCurrent, float pInterpolateRate);
	~FollowPlaneCamera();

	FollowPlaneCamera& operator=(const FollowPlaneCamera&) = delete;
	FollowPlaneCamera(FollowPlaneCamera&) = delete;

	void Update() override;
};

