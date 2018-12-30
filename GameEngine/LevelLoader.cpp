#include "LevelLoader.h"

#include "ResourceManager.h"
#include "ComponentModel.h"
#include "ComponentShader.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"
#include "ComponentPhysics.h"
#include "ComponentTexture.h"
#include "ComponentNormalTexture.h"
#include "CollisionCuboid.h"
#include "CollisionSphere.h"
#include "CollisionShape.h"
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
			LoadEntityTemplatesJSON(d["EntityTemplate"]);
		}
	}

	if (d.HasMember("Map"))
	{
		if (d["Map"].IsObject())
		{
			LoadMapJSON(d["Map"]);
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

void LevelLoader::LoadEntityTemplatesJSON(const Value& EntityTemplates)
{
	Value::ConstValueIterator it;

	for (it = EntityTemplates.Begin(); it != EntityTemplates.End(); it++)
	{
		string name = (*it)["Name"].GetString();
		const Value& components = (*it)["Components"];

		templates.insert(pair<string, const Value&>(name, components));
	}
}

void LevelLoader::LoadMapJSON(const Value& Map)
{
	string type = Map["Type"].GetString();
	
	if (type == "Platformer")
	{
		string file = Map["File"].GetString();
		string plane = Map["Plane"].GetString();
		vec2 topLeftCoord = vec2(0);

		const Value& position = Map["TopLeftCoord"];

		for (SizeType i = 0; i < position.Size(); i++)
		{
			topLeftCoord[i] = position[i].GetFloat();
		}

		LoadPlatformerMap(file, plane, topLeftCoord);
	}
}

void LevelLoader::LoadPlatformerMap(string file, string plane, vec2 topLeftCoord)
{
	int x = 0;
	int y = 0;

	EntityManager * entityManager = EntityManager::Instance();

	ifstream in;
	in.open(file);

	while (!in.eof())
	{
		char letter = in.get();

		if (letter == '\n')
		{
			x = -1;
			y--;
		}
		else
		{		
			map<string, const Value&>::iterator it = templates.find(string(1, letter));
			if (it != templates.end())
			{
				Entity * newEntity = entityManager->CreateEntity();
				AddComponentsToEntityJSON(newEntity, it->second);

				vec3 position = vec3(0);

				if (plane == "XY")
				{
					position = vec3(x, y, 0);
				}
				else if (plane == "XZ")
				{
					position = vec3(x, 0, y);
				}
				else if (plane == "ZY")
				{
					position = vec3(0, y, x);
				}
				entityManager->AddComponentToEntity(newEntity, new ComponentPosition(position));
			}
		}
		x++;
	}

	Entity * newEntity = entityManager->CreateEntity("Player");
	entityManager->AddComponentToEntity(newEntity, new ComponentModel("Sphere"));
	entityManager->AddComponentToEntity(newEntity, new ComponentShader("NormalShader"));
	entityManager->AddComponentToEntity(newEntity, new ComponentPosition(vec3(1, -1, 0)));
	entityManager->AddComponentToEntity(newEntity, new ComponentDirection(vec3(0, 0, 1), 0));
	entityManager->AddComponentToEntity(newEntity, new ComponentPhysics(new CollisionSphere(0.5f), 1, PLAYER, newEntity));
	entityManager->AddComponentToEntity(newEntity, new ComponentTexture("Earth"));
	entityManager->AddComponentToEntity(newEntity, new ComponentNormalTexture("EarthNormal"));
}

void LevelLoader::AddComponentsToEntityJSON(Entity * entity, const Value& components)
{
	EntityManager * entityManager = EntityManager::Instance();

	Value::ConstValueIterator it;

	for (it = components.Begin(); it != components.End(); it++)
	{
		string component = (*it)["Component"].GetString();

		if (component == "Model")
		{
			string model = (*it)["Model"].GetString();
			entityManager->AddComponentToEntity(entity, new ComponentModel(model));
		}
		else if (component == "Shader")
		{
			string shader = (*it)["Shader"].GetString();
			entityManager->AddComponentToEntity(entity, new ComponentShader(shader));
		}
		else if (component == "Direction")
		{
			vec3 direction = vec3(0);

			const Value& dir = (*it)["Direction"];

			for (SizeType i = 0; i < dir.Size(); i++)
			{
				direction[i] = dir[i].GetFloat();
			}

			float angle = (*it)["Angle"].GetFloat();
			entityManager->AddComponentToEntity(entity, new ComponentDirection(direction, angle));
		}
		else if (component == "Physics")
		{
			CollisionShape * collisionShape = nullptr;
			const Value& shape = (*it)["Shape"];

			string shapeType = shape["Type"].GetString();

			if (shapeType == "Sphere")
			{
				float radius = shape["Radius"].GetFloat();
				collisionShape = new CollisionSphere(radius);
			}
			else if (shapeType == "Cuboid")
			{
				vec3 size = vec3(0);

				const Value& sizeLoc = shape["Size"];

				for (SizeType i = 0; i < sizeLoc.Size(); i++)
				{
					size[i] = sizeLoc[i].GetFloat();
				}

				collisionShape = new CollisionCuboid(size);
			}

			float mass = (*it)["Mass"].GetFloat();
			string type = (*it)["Type"].GetString();

			EntityType eType;

			if (type == "WALL")
			{
				eType = EntityType::WALL;
			}
			else if (type == "COLLECTABLE")
			{
				eType = EntityType::COLLECTABLE;
			}
			else if (type == "PLAYER")
			{
				eType = EntityType::PLAYER;
			}
			else
			{
				eType = EntityType::NONE;
			}

			if ((*it).HasMember("CollisionResponse"))
			{
				bool response = (*it)["CollisionResponse"].GetBool();

				entityManager->AddComponentToEntity(entity, new ComponentPhysics(collisionShape, mass, eType, entity, response));
			}
			else
			{
				entityManager->AddComponentToEntity(entity, new ComponentPhysics(collisionShape, mass, eType, entity));
			}
		}
		else if (component == "Texture")
		{
			string texture = (*it)["Texture"].GetString();
			entityManager->AddComponentToEntity(entity, new ComponentTexture(texture));
		}
		else if (component == "NormalTexture")
		{
			string texture = (*it)["Texture"].GetString();
			entityManager->AddComponentToEntity(entity, new ComponentNormalTexture(texture));
		}
	}
}

map<string, const Value&> LevelLoader::templates;