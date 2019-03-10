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

	glm::vec3 GetPosition() override;
	glm::quat GetDirection() override;
	void * GetRigidBody() override;

	void SetPosition(glm::vec3 position) override;
	void SetDirection(glm::quat direction) override;

	void ApplyVelocity(glm::vec3 & velocity) override;
	void ApplyImpulse(glm::vec3 & impulse) override;
	void ApplyRotation(glm::vec3 & rotation) override;
};

