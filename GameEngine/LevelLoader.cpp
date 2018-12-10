#include "LevelLoader.h"

#include "ResourceManager.h"
#include "Entity.h"
#include "ComponentModel.h"
#include "ComponentShader.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"
#include "ComponentPhysics.h"
#include "ComponentTexture.h"
#include "CollisionCuboid.h"
#include "CollisionSphere.h"

#include <fstream>

void LevelLoader::LoadLevel(string fileName, EntityManager & pEntityManager)
{

	ResourceManager::LoadModel("Cube", "cube.obj");
	ResourceManager::LoadModel("Sphere", "sphere.obj");
	ResourceManager::LoadShader("TestShader", "TestVertex.vert", "TestFragment.frag");
	ResourceManager::LoadShader("NormalShader", "NormalVertex.vert", "NormalFragment.frag");
	ResourceManager::LoadShader("RiggedShader", "RiggedVertex.vert", "RiggedFragment.frag");
	ResourceManager::LoadTexture("Box", "container.jpg");
	ResourceManager::LoadTexture("BoxNormal", "containerNormal.jpg");
	ResourceManager::LoadTexture("Earth", "2k_earth_daymap.jpg");

	int x = 0;
	int y = 0;

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
			Entity * newEntity = pEntityManager.CreateEntity();
			pEntityManager.AddComponentToEntity(newEntity, new ComponentModel("Cube"));
			pEntityManager.AddComponentToEntity(newEntity, new ComponentShader("TestShader"));
			pEntityManager.AddComponentToEntity(newEntity, new ComponentPosition(vec3(x, y, -7.5f)));
			pEntityManager.AddComponentToEntity(newEntity, new ComponentDirection(vec3(0, 0, 1), 0));
			pEntityManager.AddComponentToEntity(newEntity, new ComponentPhysics(new CollisionCuboid(vec3(0.5f, 0.5f, 0.5f)), 0, WALL, newEntity));
			pEntityManager.AddComponentToEntity(newEntity, new ComponentTexture("Box"));
		}
		else if (letter == 'p')
		{
			Entity * newEntity = pEntityManager.CreateEntity("Player");
			pEntityManager.AddComponentToEntity(newEntity, new ComponentModel("Sphere"));
			pEntityManager.AddComponentToEntity(newEntity, new ComponentShader("TestShader"));
			pEntityManager.AddComponentToEntity(newEntity, new ComponentPosition(vec3(x, y, -7.5f)));
			pEntityManager.AddComponentToEntity(newEntity, new ComponentDirection(vec3(0, 0, 1), 0));
			pEntityManager.AddComponentToEntity(newEntity, new ComponentPhysics(new CollisionSphere(0.5f), 1, PLAYER, newEntity));
			pEntityManager.AddComponentToEntity(newEntity, new ComponentTexture("Earth"));
		}
		x++;
	}
}