#pragma once
#include "RigidBody.h"
#include "..\BulletPhysics\btBulletDynamicsCommon.h"

#include "glm/gtc/quaternion.hpp"

class BulletRigidBody : public RigidBody
{
private:
	btRigidBody * rigidBody;
public:
	BulletRigidBody(btRigidBody * pRigidBody) : rigidBody(pRigidBody) {};
	~BulletRigidBody();

	vec3 GetPosition() override;
	quat GetDirection() override;
	void * GetRigidBody() override;

	void ApplyVelocity(vec3 & velocity) override;
	void ApplyImpulse(vec3 & impulse) override;
	void ApplyRotation(vec3 & rotation) override;
};

