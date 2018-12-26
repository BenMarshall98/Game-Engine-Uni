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

void LevelLoader::LoadLevelJSON(string fileName)
{
	ifstream in(fileName);

	string fullFile;

	while (!in.eof())
	{
		char c = in.get();
		if (c == -1)
		{
			break;
		}
		fullFile += c;
	}

	Document d;
	d.Parse(fullFile.c_str());

	if (d.HasMember("Resources"))
	{
		if (d["Resources"].IsArray())
		{
			LoadResourcesJSON(d["Resources"]);
		}
	}

	if (d.HasMember("EntityTemplate"))
	{
		if (d["EntityTemplate"].IsArray())
		{

		}
	}
}

void LevelLoader::LoadResourcesJSON(const Value& Resources)
{
	Value::ConstValueIterator it;

	for (it = Resources.Begin(); it != Resources.End(); it++)
	{
		string type = (*it)["Type"].GetString();
		string name = (*it)["Name"].GetString();

		if (type == "Texture")
		{
			string texture = (*it)["Texture"].GetString();
			ResourceManager::LoadTexture(name, texture);
		}
		else if (type == "Model")
		{
			string model = (*it)["Model"].GetString();
			ResourceManager::LoadModel(name, model);
		}
		else if (type == "Shader")
		{
			string vertex = (*it)["Vertex"].GetString();
			string fragment = (*it)["Fragment"].GetString();

			if (it->HasMember("Geometry"))
			{
				string geometry = (*it)["Geometry"].GetString();
				ResourceManager::LoadShader(name, vertex, fragment, geometry);
			}
			else
			{
				ResourceManager::LoadShader(name, vertex, fragment);
			}
		}
	}
}