#include "LevelLoader.h"

#include "ResourceManager.h"
#include "ComponentModel.h"
#include "ComponentShader.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"
#include "ComponentPhysics.h"
#include "ComponentTexture.h"
#include "ComponentAudio.h"
#include "ComponentNormalTexture.h"
#include "ComponentRiggedAnimation.h"
#include "ComponentArtificalIntelligence.h"
#include "ComponentShadowShader.h"
#include "CollisionCuboid.h"
#include "CollisionSphere.h"
#include "CollisionShape.h"
#include "EntityManager.h"
#include "Projection.h"
#include "GLFWWindow.h"
#include "FollowPlaneCamera.h"
#include "FollowPlayerCamera.h"
#include "Camera.h"
#include "InputMapping.h"
#include "ScriptingManager.h"
#include "ComponentInput.h"
#include "CameraManager.h"
#include "RapidJSON/error/en.h"
#include "LightManager.h"
#include <fstream>
#include <iostream>

void LevelLoader::CoinHitPlayer(Entity * const pEntity)
{
	EntityManager::Instance()->AddToDeleteList(pEntity);
}

void LevelLoader::LoadLevelJSON(string & fileName)
{
	ifstream in(fileName);

	string fullFile;

	while (!in.eof())
	{
		const char c = in.get();
		if (c == -1)
		{
			break;
		}
		fullFile += c;
	}

	Document d;
	d.Parse(fullFile.c_str());

	if (d.HasParseError())
	{
		const int loc = d.GetErrorOffset();
		const string error = GetParseError_En(d.GetParseError());
		const int line = GetLine(fullFile, loc);
		return;
	}

	if (d.HasMember("Resources"))
	{
		if (d["Resources"].IsArray())
		{
			LoadResourcesJSON(d["Resources"]);
		}
	}

	if (d.HasMember("Scripts"))
	{
		if (d["Scripts"].IsArray())
		{
			LoadScriptsJSON(d["Scripts"]);
		}
	}

	if (d.HasMember("EntityTemplate"))
	{
		if (d["EntityTemplate"].IsArray())
		{
			LoadEntityTemplatesJSON(d["EntityTemplate"]);
		}
	}

	if (d.HasMember("Entities"))
	{
		if (d["Entities"].IsArray())
		{
			LoadEntity(d["Entities"]);
		}
	}

	if (d.HasMember("Map"))
	{
		if (d["Map"].IsObject())
		{
			LoadMapJSON(d["Map"]);
		}
	}

	if (d.HasMember("Lights"))
	{
		if (d["Lights"].IsArray())
		{
			LoadLights(d["Lights"]);
		}
	}
}

void LevelLoader::LoadResourcesJSON(const Value& Resources)
{
	Value::ConstValueIterator it;

	for (it = Resources.Begin(); it != Resources.End(); it++)
	{
		const string type = (*it)["Type"].GetString();
		const string name = (*it)["Name"].GetString();

		if (type == "Texture")
		{
			const string texture = (*it)["Texture"].GetString();
			ResourceManager::LoadTexture(name, texture);
		}
		else if (type == "Model")
		{
			const string model = (*it)["Model"].GetString();
			ResourceManager::LoadModel(name, model);
		}
		else if (type == "Shader")
		{
			const string vertex = (*it)["Vertex"].GetString();
			const string fragment = (*it)["Fragment"].GetString();

			if (it->HasMember("Geometry"))
			{
				const string geometry = (*it)["Geometry"].GetString();
				ResourceManager::LoadShader(name, vertex, fragment, geometry);
			}
			else
			{
				ResourceManager::LoadShader(name, vertex, fragment);
			}
		}
		else if (type == "Audio")
		{
			const string audio = (*it)["Audio"].GetString();
			ResourceManager::LoadAudio(name, audio);
		}
	}
}

void LevelLoader::LoadScriptsJSON(const Value& Scripts)
{
	ScriptingManager * const scriptingManager = ScriptingManager::Instance();

	Value::ConstValueIterator it;

	for (it = Scripts.Begin(); it != Scripts.End(); it++)
	{
		const string script = (*it).GetString();

		scriptingManager->LoadLuaFromFile(script);
	}
}

void LevelLoader::LoadEntityTemplatesJSON(const Value& EntityTemplates)
{
	Value::ConstValueIterator it;

	for (it = EntityTemplates.Begin(); it != EntityTemplates.End(); it++)
	{
		const string name = (*it)["Name"].GetString();
		const Value& components = (*it)["Components"];

		templates.insert(pair<string, const Value&>(name, components));
	}
}

void LevelLoader::LoadEntity(const Value& Entities)
{
	Value::ConstValueIterator it;

	EntityManager *  const entityManager = EntityManager::Instance();

	for (it = Entities.Begin(); it != Entities.End(); it++)
	{
		const string name = (*it)["Name"].GetString();

		Entity * const entity = entityManager->CreateEntity(name);

		if ((*it).HasMember("Template"))
		{
			const string templateName = (*it)["Template"].GetString();

			const map<string, const Value&>::iterator it = templates.find(templateName);
			if (it != templates.end())
			{
				AddComponentsToEntityJSON(entity, it->second);
			}
		}

		AddComponentsToEntityJSON(entity, (*it)["Components"]);
	}
}

void LevelLoader::LoadLights(const Value& Lights)
{
	Value::ConstValueIterator it;

	LightManager * const lightManager = LightManager::Instance();

	for (it = Lights.Begin(); it != Lights.End(); it++)
	{
		const string type = (*it)["Type"].GetString();

		vec3 colour = vec3(0);

		const Value& col = (*it)["Colour"];

		for (SizeType i = 0; i < col.Size(); i++)
		{
			colour[i] = col[i].GetFloat();
		}

		if (type == "Directional")
		{
			vec3 direction = vec3(0);

			const Value& dir = (*it)["Direction"];

			for (SizeType i = 0; i < dir.Size(); i++)
			{
				direction[i] = dir[i].GetFloat();
			}

			lightManager->SetDirectionalLight(direction, colour);
		}
		else if (type == "Point")
		{
			vec3 location = vec3(0);

			const Value& loc = (*it)["Location"];

			for (SizeType i = 0; i < loc.Size(); i++)
			{
				location[i] = loc[i].GetFloat();
			}

			lightManager->AddPointLight(location, colour);
		}
		else if (type == "Spot")
		{
			vec3 location = vec3(0);

			const Value& loc = (*it)["Location"];

			for (SizeType i = 0; i < loc.Size(); i++)
			{
				location[i] = loc[i].GetFloat();
			}
			
			vec3 direction = vec3(0);

			const Value& dir = (*it)["Direction"];

			for (SizeType i = 0; i < dir.Size(); i++)
			{
				direction[i] = dir[i].GetFloat();
			}

			const float innerCone = (*it)["InnerCone"].GetFloat();
			const float outerCone = (*it)["OuterCone"].GetFloat();

			lightManager->AddSpotLight(location, direction, colour, innerCone, outerCone);
		}
	}
}

void LevelLoader::LoadMapJSON(const Value& Map)
{
	const string type = Map["Type"].GetString();

	const string file = Map["File"].GetString();
	vec2 topLeftCoord = vec2(0);

	const Value& position = Map["TopLeftCoord"];

	for (SizeType i = 0; i < position.Size(); i++)
	{
		topLeftCoord[i] = position[i].GetFloat();
	}
	
	if (type == "Platformer")
	{
		LoadPlatformerMap(file, topLeftCoord);
	}
	else if (type == "3DLevel")
	{
		Load3DMap(file, topLeftCoord);
	}

	if (Map.HasMember("View"))
	{
		if (Map["View"].IsObject())
		{
			LoadViewJSON(Map["View"]);
		}
	}
}

void LevelLoader::LoadViewJSON(const Value& View)
{
	if (View.HasMember("Perspective"))
	{
		if (View["Perspective"].IsObject())
		{
			LoadPerspectiveJSON(View["Perspective"]);
		}
	}

	if (View.HasMember("Camera"))
	{
		if (View["Camera"].IsObject())
		{
			LoadCameraJSON(View["Camera"]);
		}
	}
}

void LevelLoader::LoadPerspectiveJSON(const Value& Perspective)
{
	const string type = Perspective["Type"].GetString();

	const float minDist = Perspective["MinDistance"].GetFloat();
	const float maxDist = Perspective["MaxDistance"].GetFloat();

	//TODO: get projection into a camera manager

	if (type == "Perspective")
	{
		Projection * const projection = new Projection(ProjectionType::Perspective, GLFWWindow::GetWidth(), GLFWWindow::GetHeight(),  minDist, maxDist);
		CameraManager::Instance()->SetProjection(projection);
	}
	else if (type == "Orthographic")
	{
		Projection * const projection = new Projection(ProjectionType::Orthographic, GLFWWindow::GetWidth(), GLFWWindow::GetHeight(), minDist, maxDist);
		CameraManager::Instance()->SetProjection(projection);
	}
}

void LevelLoader::LoadCameraJSON(const Value& pCamera)
{
	//TODO: get camera into a camera manager

	const string type = pCamera["Type"].GetString();

	const float minDist = pCamera["MinDistance"].GetFloat();
	const float maxDist = pCamera["MaxDistance"].GetFloat();
	const float defDist = pCamera["DefaultDistance"].GetFloat();
	const float followRate = pCamera["FollowRate"].GetFloat();
	const string follow = pCamera["Follow"].GetString();

	Entity * const entity = EntityManager::Instance()->GetEntityByName(follow);

	if (type == "FollowPlane")
	{
		Camera * const camera = new FollowPlaneCamera(entity, minDist, maxDist, defDist, followRate);
		CameraManager::Instance()->SetCamera(camera);
	}
	else if (type == "FollowPlayer")
	{
		Camera * const camera = new FollowPlayerCamera(entity, minDist, maxDist, defDist, followRate);
		CameraManager::Instance()->SetCamera(camera);
	}
	
}

void LevelLoader::LoadPlatformerMap(const string & file, const vec2 & topLeftCoord)
{
	int x = topLeftCoord.x;
	int y = topLeftCoord.y;

	EntityManager * const entityManager = EntityManager::Instance();

	ifstream in;
	in.open(file);

	while (!in.eof())
	{
		const char letter = in.get();

		if (letter == '\n')
		{
			x = -1;
			y--;
		}
		else
		{		
			const map<string, const Value&>::iterator it = templates.find(string(1, letter));
			if (it != templates.end())
			{
				Entity * const newEntity = entityManager->CreateEntity();
				AddComponentsToEntityJSON(newEntity, it->second);

				vec3 position = position = vec3(x, y, 0);

				entityManager->AddComponentToEntity(newEntity, new ComponentPosition(position));
			}
		}
		x++;
	}
}

void LevelLoader::Load3DMap(const string & file, const vec2 & topLeftCoord)
{
	float x = topLeftCoord.x;
	float y = topLeftCoord.y;

	EntityManager * const entityManager = EntityManager::Instance();

	ifstream in;
	in.open(file);

	while (!in.eof())
	{
		const char letter = in.get();

		if (letter == '\n')
		{
			x = topLeftCoord.x -1;
			y--;
		}
		else
		{
			const map<string, const Value&>::iterator it = templates.find(string(1, letter));
			if (it != templates.end())
			{
				Entity * const newEntity = entityManager->CreateEntity();
				AddComponentsToEntityJSON(newEntity, it->second);

				vec3 position = position = vec3(x, 0.5, y);

				entityManager->AddComponentToEntity(newEntity, new ComponentPosition(position));
			}
		}
		x++;
	}
}

void LevelLoader::AddComponentsToEntityJSON(Entity * entity, const Value& components)
{
	EntityManager * const entityManager = EntityManager::Instance();

	Value::ConstValueIterator it;

	for (it = components.Begin(); it != components.End(); it++)
	{
		const string component = (*it)["Component"].GetString();

		if (component == "Model")
		{
			const string model = (*it)["Model"].GetString();
			entityManager->AddComponentToEntity(entity, new ComponentModel(model));
		}
		else if (component == "Shader")
		{
			const string shader = (*it)["Shader"].GetString();
			entityManager->AddComponentToEntity(entity, new ComponentShader(shader));
		}
		else if (component == "Position")
		{
			vec3 position = vec3(0);

			const Value& pos = (*it)["Position"];

			for (SizeType i = 0; i < pos.Size(); i++)
			{
				position[i] = pos[i].GetFloat();
			}

			entityManager->AddComponentToEntity(entity, new ComponentPosition(position));
		}
		else if (component == "Direction")
		{
			vec3 direction = vec3(0);

			const Value& dir = (*it)["Direction"];

			for (SizeType i = 0; i < dir.Size(); i++)
			{
				direction[i] = dir[i].GetFloat();
			}

			const float angle = (*it)["Angle"].GetFloat();
			entityManager->AddComponentToEntity(entity, new ComponentDirection(direction, angle));
		}
		else if (component == "ArtificialIntelligence")
		{
			PathFollowing * pathfollower = nullptr;
			PathFinding * pathFinding = nullptr;

			if ((*it).HasMember("PathFollowing"))
			{
				vector<PathNode *> pathNodes;
				Value::ConstValueIterator path;

				for (path = (*it)["PathFollowing"].Begin(); path != (*it)["PathFollowing"].End(); path++)
				{
					vec3 position = vec3(0);

					const Value& pos = (*path)["Position"];

					for (SizeType i = 0; i < pos.Size(); i++)
					{
						position[i] = pos[i].GetFloat();
					}

					const float radius = (*path)["Radius"].GetFloat();

					PathNode * const node = new PathNode();
					node->position = position;
					node->radius = radius;

					pathNodes.push_back(node);
				}

				pathfollower = new PathFollowing(pathNodes);
			}
			
			if ((*it).HasMember("PathFinding"))
			{
				const Value& pathFind = (*it)["PathFinding"];

				string target = pathFind["Target"].GetString();
				string map = pathFind["Map"].GetString();

				vec2 position = vec3(0);

				const Value& pos = pathFind["TopLeftCoord"];

				for (SizeType i = 0; i < pos.Size(); i++)
				{
					position[i] = pos[i].GetFloat();
				}

				pathFinding = new PathFinding(target, map, position);
			}

			entityManager->AddComponentToEntity(entity, new ComponentArtificalIntelligence(pathfollower, pathFinding));
		}
		else if (component == "Audio")
		{
			const string name = (*it)["Audio"].GetString();
			const string playback = (*it)["Playback"].GetString();

			if (playback == "Play")
			{
				entityManager->AddComponentToEntity(entity, new ComponentAudio(AudioPlayback::PLAY, name));
			}
			else if (playback == "Pause")
			{
				entityManager->AddComponentToEntity(entity, new ComponentAudio(AudioPlayback::PAUSE, name));
			}
			else if (playback == "Stop")
			{
				entityManager->AddComponentToEntity(entity, new ComponentAudio(AudioPlayback::STOP, name));
			}
		}
		else if (component == "Physics")
		{
			CollisionShape * collisionShape = nullptr;
			const Value& shape = (*it)["Shape"];

			const string shapeType = shape["Type"].GetString();

			if (shapeType == "Sphere")
			{
				const float radius = shape["Radius"].GetFloat();
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

			const float mass = (*it)["Mass"].GetFloat();
			const string type = (*it)["Type"].GetString();

			vec3 angularLimit = vec3(0);

			const Value& angLimit = (*it)["AngularLimit"];

			for (SizeType i = 0; i < angLimit.Size(); i++)
			{
				angularLimit[i] = angLimit[i].GetFloat();
			}

			EntityType eType = ComponentPhysics::StringToEnum(type);

			if ((*it).HasMember("CollisionResponse"))
			{
				bool response = (*it)["CollisionResponse"].GetBool();

				if ((*it).HasMember("CollisionFunctions"))
				{
					map<EntityType, string> * collisionFunctions = new map<EntityType, string>();

					Value::ConstValueIterator col;

					for (col = (*it)["CollisionFunctions"].Begin(); col != (*it)["CollisionFunctions"].End(); col++)
					{
						const string entityType = (*col)["EntityType"].GetString();
						const string function = (*col)["Function"].GetString();

						EntityType cEntityType = ComponentPhysics::StringToEnum(entityType);

						collisionFunctions->insert(pair<EntityType, string>(cEntityType, function));
					}

					entityManager->AddComponentToEntity(entity, new ComponentPhysics(collisionShape, mass, eType, entity, angularLimit, response, collisionFunctions));
				}

				entityManager->AddComponentToEntity(entity, new ComponentPhysics(collisionShape, mass, eType, entity, angularLimit, response));
			}
			else
			{
				entityManager->AddComponentToEntity(entity, new ComponentPhysics(collisionShape, mass, eType, entity, angularLimit));
			}
		}
		else if (component == "Texture")
		{
			const string texture = (*it)["Texture"].GetString();
			entityManager->AddComponentToEntity(entity, new ComponentTexture(texture));
		}
		else if (component == "NormalTexture")
		{
			const string texture = (*it)["Texture"].GetString();
			entityManager->AddComponentToEntity(entity, new ComponentNormalTexture(texture));
		}
		else if (component == "RiggedAnimation")
		{
			const string animation = (*it)["Animation"].GetString();
			const string playback = (*it)["Playback"].GetString();
			RiggedAnimationState playbackState;

			if (playback == "Play")
			{
				playbackState = RiggedAnimationState::PLAY;
			}
			else if (playback == "Pause")
			{
				playbackState = RiggedAnimationState::PAUSE;
			}
			else
			{
				playbackState = RiggedAnimationState::STOP;
			}

			entityManager->AddComponentToEntity(entity, new ComponentRiggedAnimation(animation, playbackState));
		}
		else if (component == "ShadowShader")
		{
			string direction = (*it)["Direction"].GetString();
			string point = (*it)["Point"].GetString();

			entityManager->AddComponentToEntity(entity, new ComponentShadowShader(direction, point));
		}
		else if (component == "Input")
		{
			vector<InputFunction> * const playerInputs = LoadInputsJSON((*it)["Inputs"]);
			entityManager->AddComponentToEntity(entity, new ComponentInput(playerInputs));
		}
	}
}

vector<InputFunction> * LevelLoader::LoadInputsJSON(const Value& Inputs)
{
	vector<InputFunction> * playerInputs = new vector<InputFunction>();

	Value::ConstValueIterator it;

	for (it = Inputs.Begin(); it != Inputs.End(); it++)
	{
		const string function = (*it)["Function"].GetString();

		InputFunction inputFunction = InputFunction(function);

		const Value& keys = (*it)["Input"];

		Value::ConstValueIterator key;

		for (key = keys.Begin(); key != keys.End(); key++)
		{
			const string inputName = key->GetString();
			const GameInput gameInput = InputConverter::StringToEnum(inputName);
			inputFunction.AddInput(gameInput);
		}

		playerInputs->push_back(inputFunction);
	}

	return playerInputs;
}

int LevelLoader::GetLine(string & file, const int location)
{
	int count = 0;

	for (int i = 0; i < location; i++)
	{
		if (file[i] == '\n')
		{
			count++;
		}
	}

	return count;
}

map<string, const Value&> LevelLoader::templates;