#pragma once
#include "Camera.h"
#include "Entity.h"

enum Plane
{
	XY,
	XZ,
	ZY
};
class FollowPlaneCamera : public Camera
{
private:
	Entity * mEntity;
	Plane mPlane;
	float mDistanceMin;
	float mDistanceMax;
	float mDistanceCurrent;
	float mInterpolateRate;
	vec3 lastPosition;

public:
	FollowPlaneCamera(Entity * pEntity, Plane pPlane, float pDistanceMin, float pDistanceMax, float pDistanceCurrent, float pInterpolateRate);
	~FollowPlaneCamera();

	void Update() override;
};

