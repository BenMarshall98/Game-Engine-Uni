#include "LevelLoader.h"

#include "ResourceManager.h"
#include "Entity.h"
#include "ComponentModel.h"
#include "ComponentShader.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"
#include "ComponentPhysics.h"
#include "ComponentTexture.h"
#include "ComponentNormalTexture.h"
#include "CollisionCuboid.h"
#include "CollisionSphere.h"
#include "EntityManager.h"

#include <fstream>
#include <iostream>

void LevelLoader::LoadLevel(string fileName)
{

	ResourceManager::LoadModel("Cube", "cube.obj");
	ResourceManager::LoadModel("Sphere", "sphere.obj");
	ResourceManager::LoadModel("Collectable", "Collectable.obj");
	ResourceManager::LoadShader("TestShader", "TestVertex.vert", "TestFragment.frag");
	ResourceManager::LoadShader("NormalShader", "NormalVertex.vert", "NormalFragment.frag");
	ResourceManager::LoadShader("RiggedShader", "RiggedVertex.vert", "RiggedFragment.frag");
	ResourceManager::LoadTexture("Box", "container.jpg");
	ResourceManager::LoadTexture("BoxNormal", "containerNormal.jpg");
	ResourceManager::LoadTexture("Earth", "2k_earth_daymap.jpg");
	ResourceManager::LoadTexture("EarthNormal", "Earth_Normal.jpg");

	int x = 0;
	int y = 0;

	EntityManager * entityManager = EntityManager::Instance();

	ifstream in;
	in.open(fileName);

	while (!in.eof())
	{
		char letter = in.get();

		if (letter == '\n')
		{
			x = -1;
			y--;
		}
		else if (letter == 'w')
		{
			Entity * newEntity = entityManager->CreateEntity();
			entityManager->AddComponentToEntity(newEntity, new ComponentModel("Cube"));
			entityManager->AddComponentToEntity(newEntity, new ComponentShader("NormalShader"));
			entityManager->AddComponentToEntity(newEntity, new ComponentPosition(vec3(x, y, -7.5f)));
			entityManager->AddComponentToEntity(newEntity, new ComponentDirection(vec3(0, 0, 1), 0));
			entityManager->AddComponentToEntity(newEntity, new ComponentPhysics(new CollisionCuboid(vec3(0.5f, 0.5f, 0.5f)), 0, WALL, newEntity));
			entityManager->AddComponentToEntity(newEntity, new ComponentTexture("Box"));
			entityManager->AddComponentToEntity(newEntity, new ComponentNormalTexture("BoxNormal"));
		}
		else if (letter == 'p')
		{
			Entity * newEntity = entityManager->CreateEntity("Player");
			entityManager->AddComponentToEntity(newEntity, new ComponentModel("Sphere"));
			entityManager->AddComponentToEntity(newEntity, new ComponentShader("NormalShader"));
			entityManager->AddComponentToEntity(newEntity, new ComponentPosition(vec3(x, y, -7.5f)));
			entityManager->AddComponentToEntity(newEntity, new ComponentDirection(vec3(0, 0, 1), 0));
			entityManager->AddComponentToEntity(newEntity, new ComponentPhysics(new CollisionSphere(0.5f), 1, PLAYER, newEntity));
			entityManager->AddComponentToEntity(newEntity, new ComponentTexture("Earth"));
			entityManager->AddComponentToEntity(newEntity, new ComponentNormalTexture("EarthNormal"));
		}
		else if (letter == 'c')
		{
			map<EntityType, GameCollisionFunction> * collisionFunctions = new map<EntityType, GameCollisionFunction>();
			collisionFunctions->insert(pair<EntityType, GameCollisionFunction>(PLAYER, CoinHitPlayer));

			Entity * newEntity = entityManager->CreateEntity();
			entityManager->AddComponentToEntity(newEntity, new ComponentModel("Collectable"));
			entityManager->AddComponentToEntity(newEntity, new ComponentShader("TestShader"));
			entityManager->AddComponentToEntity(newEntity, new ComponentPosition(vec3(x, y, -7.5f)));
			entityManager->AddComponentToEntity(newEntity, new ComponentDirection(vec3(0, 0, 1), 0));
			entityManager->AddComponentToEntity(newEntity, new ComponentPhysics(new CollisionSphere(0.25f), 0, COLLECTABLE, newEntity, false, collisionFunctions));
			entityManager->AddComponentToEntity(newEntity, new ComponentTexture("Earth"));
		}
		x++;
	}
}

void LevelLoader::CoinHitPlayer(Entity * pEntity)
{
	EntityManager::Instance()->AddToDeleteList(pEntity);
}