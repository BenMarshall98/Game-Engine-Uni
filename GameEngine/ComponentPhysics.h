#pragma once
#include "iComponent.h"
#include "CollisionShape.h"
#include "glm/glm.hpp"

using namespace glm;

class ComponentPhysics : public iComponent
{
private:
	CollisionShape * shape;
	vec3 force = vec3(0);
	vec3 impulse = vec3(0);
	float rotation = 0;
	float mass;
	void* rigidBody;

public:
	ComponentPhysics(CollisionShape * pShape, float pMass) : shape(pShape), mass(pMass) {}
	~ComponentPhysics() {}

	ComponentType GetComponentName();
	void Swap() {}

	inline vec3 GetForce()
	{
		return force;
	}

	inline void SetForce(vec3 pForce)
	{
		force = pForce;
	}

	inline vec3 GetImpulse()
	{
		return impulse;
	}

	inline void SetImpulse(vec3 pImpulse)
	{
		impulse = pImpulse;
	}

	inline float GetRotation()
	{
		return rotation;
	}

	inline void SetRotation(float pRotation)
	{
		rotation = pRotation;
	}

	inline CollisionShape* GetShape()
	{
		return shape;
	}

	inline float GetMass()
	{
		return mass;
	}

	inline void SetRigidBody(void* pRigidBody)
	{
		rigidBody = pRigidBody;
	}

	inline void* GetRigidBody()
	{
		return rigidBody;
	}
};

