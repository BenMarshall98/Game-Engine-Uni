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
#include "SystemManager.h"
#include "ArtificialIntelligenceSystem.h"
#include "RiggedAnimationSystem.h"
#include "RenderSystem.h"
#include "ShadowSystem.h"
#include "InputSystem.h"
#include "ArtificialIntelligenceSystem.h"
#include "PhysicsSystem.h"
#include "AudioSystem.h"
#include <fstream>
#include <iostream>

void LevelLoader::CoinHitPlayer(Entity * const pEntity)
{
	EntityManager::Instance()->AddToDeleteList(pEntity);
}

void LevelLoader::LoadLevelJSON(std::string & fileName)
{
	std::ifstream in(fileName);

	std::string fullFile;

	while (!in.eof())
	{
		const char c = in.get();
		if (c == -1)
		{
			break;
		}
		fullFile += c;
	}

	rapidjson::Document d;
	d.Parse(fullFile.c_str());

	if (d.HasParseError())
	{
		const int loc = d.GetErrorOffset();
		const std::string error = GetParseError_En(d.GetParseError());
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

	if (d.HasMember("Systems"))
	{
		if (d["Systems"].IsObject())
		{
			LoadSystems(d["Systems"]);
		}
	}
}

void LevelLoader::LoadResourcesJSON(const rapidjson::Value& Resources)
{
	rapidjson::Value::ConstValueIterator it;

	for (it = Resources.Begin(); it != Resources.End(); it++)
	{
		const std::string type = (*it)["Type"].GetString();
		const std::string name = (*it)["Name"].GetString();

		if (type == "Texture")
		{
			const std::string texture = (*it)["Texture"].GetString();
			ResourceManager::LoadTexture(name, texture);
		}
		else if (type == "Model")
		{
			const std::string model = (*it)["Model"].GetString();
			ResourceManager::LoadModel(name, model);
		}
		else if (type == "Shader")
		{
			const std::string vertex = (*it)["Vertex"].GetString();
			const std::string fragment = (*it)["Fragment"].GetString();

			if (it->HasMember("Geometry"))
			{
				const std::string geometry = (*it)["Geometry"].GetString();
				ResourceManager::LoadShader(name, vertex, fragment, geometry);
			}
			else
			{
				ResourceManager::LoadShader(name, vertex, fragment);
			}
		}
		else if (type == "Audio")
		{
			const std::string audio = (*it)["Audio"].GetString();
			ResourceManager::LoadAudio(name, audio);
		}
	}
}

void LevelLoader::LoadScriptsJSON(const rapidjson::Value& Scripts)
{
	ScriptingManager * const scriptingManager = ScriptingManager::Instance();

	rapidjson::Value::ConstValueIterator it;

	for (it = Scripts.Begin(); it != Scripts.End(); it++)
	{
		const std::string script = (*it).GetString();

		scriptingManager->LoadLuaFromFile(script);
	}
}

void LevelLoader::LoadEntityTemplatesJSON(const rapidjson::Value& EntityTemplates)
{
	rapidjson::Value::ConstValueIterator it;

	for (it = EntityTemplates.Begin(); it != EntityTemplates.End(); it++)
	{
		const std::string name = (*it)["Name"].GetString();
		const rapidjson::Value& components = (*it)["Components"];

		templates.insert(std::pair<std::string, const rapidjson::Value&>(name, components));
	}
}

void LevelLoader::LoadEntity(const rapidjson::Value& Entities)
{
	rapidjson::Value::ConstValueIterator it;

	EntityManager *  const entityManager = EntityManager::Instance();

	for (it = Entities.Begin(); it != Entities.End(); it++)
	{
		const std::string name = (*it)["Name"].GetString();

		Entity * const entity = entityManager->CreateEntity(name);

		if ((*it).HasMember("Template"))
		{
			const std::string templateName = (*it)["Template"].GetString();

			const std::map<std::string, const rapidjson::Value&>::iterator it = templates.find(templateName);
			if (it != templates.end())
			{
				AddComponentsToEntityJSON(entity, it->second);
			}
		}

		AddComponentsToEntityJSON(entity, (*it)["Components"]);
	}
}

void LevelLoader::LoadLights(const rapidjson::Value& Lights)
{
	rapidjson::Value::ConstValueIterator it;

	LightManager * const lightManager = LightManager::Instance();

	for (it = Lights.Begin(); it != Lights.End(); it++)
	{
		const std::string type = (*it)["Type"].GetString();

		glm::vec3 colour = glm::vec3(0);

		const rapidjson::Value& col = (*it)["Colour"];

		for (rapidjson::SizeType i = 0; i < col.Size(); i++)
		{
			colour[i] = col[i].GetFloat();
		}

		if (type == "Directional")
		{
			glm::vec3 direction = glm::vec3(0);

			const rapidjson::Value& dir = (*it)["Direction"];

			for (rapidjson::SizeType i = 0; i < dir.Size(); i++)
			{
				direction[i] = dir[i].GetFloat();
			}

			lightManager->SetDirectionalLight(direction, colour);
		}
		else if (type == "Point")
		{
			glm::vec3 location = glm::vec3(0);

			const rapidjson::Value& loc = (*it)["Location"];

			for (rapidjson::SizeType i = 0; i < loc.Size(); i++)
			{
				location[i] = loc[i].GetFloat();
			}

			lightManager->AddPointLight(location, colour);
		}
		else if (type == "Spot")
		{
			glm::vec3 location = glm::vec3(0);

			const rapidjson::Value& loc = (*it)["Location"];

			for (rapidjson::SizeType i = 0; i < loc.Size(); i++)
			{
				location[i] = loc[i].GetFloat();
			}
			
			glm::vec3 direction = glm::vec3(0);

			const rapidjson::Value& dir = (*it)["Direction"];

			for (rapidjson::SizeType i = 0; i < dir.Size(); i++)
			{
				direction[i] = dir[i].GetFloat();
			}

			const float innerCone = (*it)["InnerCone"].GetFloat();
			const float outerCone = (*it)["OuterCone"].GetFloat();

			lightManager->AddSpotLight(location, direction, colour, innerCone, outerCone);
		}
	}
}

void LevelLoader::LoadMapJSON(const rapidjson::Value& Map)
{
	const std::string type = Map["Type"].GetString();

	const std::string file = Map["File"].GetString();
	glm::vec2 topLeftCoord = glm::vec2(0);

	const rapidjson::Value& position = Map["TopLeftCoord"];

	for (rapidjson::SizeType i = 0; i < position.Size(); i++)
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

void LevelLoader::LoadViewJSON(const rapidjson::Value& View)
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

void LevelLoader::LoadPerspectiveJSON(const rapidjson::Value& Perspective)
{
	const std::string type = Perspective["Type"].GetString();

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

void LevelLoader::LoadCameraJSON(const rapidjson::Value& pCamera)
{
	//TODO: get camera into a camera manager

	const std::string type = pCamera["Type"].GetString();

	const float minDist = pCamera["MinDistance"].GetFloat();
	const float maxDist = pCamera["MaxDistance"].GetFloat();
	const float defDist = pCamera["DefaultDistance"].GetFloat();
	const float followRate = pCamera["FollowRate"].GetFloat();
	const std::string follow = pCamera["Follow"].GetString();

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

void LevelLoader::LoadPlatformerMap(const std::string & file, const glm::vec2 & topLeftCoord)
{
	int x = topLeftCoord.x;
	int y = topLeftCoord.y;

	EntityManager * const entityManager = EntityManager::Instance();

	std::ifstream in;
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
			const std::map<std::string, const rapidjson::Value&>::iterator it = templates.find(std::string(1, letter));
			if (it != templates.end())
			{
				Entity * const newEntity = entityManager->CreateEntity();
				AddComponentsToEntityJSON(newEntity, it->second);

				glm::vec3 position = position = glm::vec3(x, y, 0);

				entityManager->AddComponentToEntity(newEntity, new ComponentPosition(position));
			}
		}
		x++;
	}
}

void LevelLoader::Load3DMap(const std::string & file, const glm::vec2 & topLeftCoord)
{
	float x = topLeftCoord.x;
	float y = topLeftCoord.y;

	EntityManager * const entityManager = EntityManager::Instance();

	std::ifstream in;
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
			const std::map<std::string, const rapidjson::Value&>::iterator it = templates.find(std::string(1, letter));
			if (it != templates.end())
			{
				Entity * const newEntity = entityManager->CreateEntity();
				AddComponentsToEntityJSON(newEntity, it->second);

				glm::vec3 position = position = glm::vec3(x, 0.5, y);

				entityManager->AddComponentToEntity(newEntity, new ComponentPosition(position));
			}
		}
		x++;
	}
}

void LevelLoader::AddComponentsToEntityJSON(Entity * entity, const rapidjson::Value& components)
{
	EntityManager * const entityManager = EntityManager::Instance();

	rapidjson::Value::ConstValueIterator it;

	for (it = components.Begin(); it != components.End(); it++)
	{
		const std::string component = (*it)["Component"].GetString();

		if (component == "Model")
		{
			const std::string model = (*it)["Model"].GetString();
			entityManager->AddComponentToEntity(entity, new ComponentModel(model));
		}
		else if (component == "Shader")
		{
			const std::string shader = (*it)["Shader"].GetString();
			entityManager->AddComponentToEntity(entity, new ComponentShader(shader));
		}
		else if (component == "Position")
		{
			glm::vec3 position = glm::vec3(0);

			const rapidjson::Value& pos = (*it)["Position"];

			for (rapidjson::SizeType i = 0; i < pos.Size(); i++)
			{
				position[i] = pos[i].GetFloat();
			}

			entityManager->AddComponentToEntity(entity, new ComponentPosition(position));
		}
		else if (component == "Direction")
		{
			glm::vec3 direction = glm::vec3(0);

			const rapidjson::Value& dir = (*it)["Direction"];

			for (rapidjson::SizeType i = 0; i < dir.Size(); i++)
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
			AIStateMachine * stateMachine = nullptr;
			std::string target = "";

			if ((*it).HasMember("Target"))
			{
				target = (*it)["Target"].GetString();
			}

			if ((*it).HasMember("PathFollowing"))
			{
				std::vector<PathNode *> pathNodes;
				rapidjson::Value::ConstValueIterator path;

				for (path = (*it)["PathFollowing"].Begin(); path != (*it)["PathFollowing"].End(); path++)
				{
					glm::vec3 position = glm::vec3(0);

					const rapidjson::Value& pos = (*path)["Position"];

					for (rapidjson::SizeType i = 0; i < pos.Size(); i++)
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
				const rapidjson::Value& pathFind = (*it)["PathFinding"];
				std::string map = pathFind["Map"].GetString();

				glm::vec2 position = glm::vec3(0);

				const rapidjson::Value& pos = pathFind["TopLeftCoord"];

				for (rapidjson::SizeType i = 0; i < pos.Size(); i++)
				{
					position[i] = pos[i].GetFloat();
				}

				pathFinding = new PathFinding(target, map, position);
			}

			if ((*it).HasMember("StateMachine"))
			{
				const rapidjson::Value& state = (*it)["StateMachine"];
				std::string startFunction = state["Function"].GetString();

				stateMachine = new AIStateMachine(startFunction);
			}

			entityManager->AddComponentToEntity(entity, new ComponentArtificalIntelligence(pathfollower, pathFinding, stateMachine, target));
		}
		else if (component == "Audio")
		{
			const std::string name = (*it)["Audio"].GetString();
			const std::string playback = (*it)["Playback"].GetString();

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
			const rapidjson::Value& shape = (*it)["Shape"];

			const std::string shapeType = shape["Type"].GetString();

			if (shapeType == "Sphere")
			{
				const float radius = shape["Radius"].GetFloat();
				collisionShape = new CollisionSphere(radius);
			}
			else if (shapeType == "Cuboid")
			{
				glm::vec3 size = glm::vec3(0);

				const rapidjson::Value& sizeLoc = shape["Size"];

				for (rapidjson::SizeType i = 0; i < sizeLoc.Size(); i++)
				{
					size[i] = sizeLoc[i].GetFloat();
				}

				collisionShape = new CollisionCuboid(size);
			}

			const float mass = (*it)["Mass"].GetFloat();
			const std::string type = (*it)["Type"].GetString();

			glm::vec3 angularLimit = glm::vec3(0);

			const rapidjson::Value& angLimit = (*it)["AngularLimit"];

			for (rapidjson::SizeType i = 0; i < angLimit.Size(); i++)
			{
				angularLimit[i] = angLimit[i].GetFloat();
			}

			EntityType eType = ComponentPhysics::StringToEnum(type);

			if ((*it).HasMember("CollisionResponse"))
			{
				bool response = (*it)["CollisionResponse"].GetBool();

				if ((*it).HasMember("CollisionFunctions"))
				{
					std::map<EntityType, std::string> * collisionFunctions = new std::map<EntityType, std::string>();

					rapidjson::Value::ConstValueIterator col;

					for (col = (*it)["CollisionFunctions"].Begin(); col != (*it)["CollisionFunctions"].End(); col++)
					{
						const std::string entityType = (*col)["EntityType"].GetString();
						const std::string function = (*col)["Function"].GetString();

						EntityType cEntityType = ComponentPhysics::StringToEnum(entityType);

						collisionFunctions->insert(std::pair<EntityType, std::string>(cEntityType, function));
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
			const std::string texture = (*it)["Texture"].GetString();
			entityManager->AddComponentToEntity(entity, new ComponentTexture(texture));
		}
		else if (component == "NormalTexture")
		{
			const std::string texture = (*it)["Texture"].GetString();
			entityManager->AddComponentToEntity(entity, new ComponentNormalTexture(texture));
		}
		else if (component == "RiggedAnimation")
		{
			const std::string animation = (*it)["Animation"].GetString();
			const std::string playback = (*it)["Playback"].GetString();
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
			std::string direction = (*it)["Direction"].GetString();
			std::string point = (*it)["Point"].GetString();

			entityManager->AddComponentToEntity(entity, new ComponentShadowShader(direction, point));
		}
		else if (component == "Input")
		{
			std::vector<InputFunction> * const playerInputs = LoadInputsJSON((*it)["Inputs"]);
			entityManager->AddComponentToEntity(entity, new ComponentInput(playerInputs));
		}
	}
}

std::vector<InputFunction> * LevelLoader::LoadInputsJSON(const rapidjson::Value& Inputs)
{
	std::vector<InputFunction> * playerInputs = new std::vector<InputFunction>();

	rapidjson::Value::ConstValueIterator it;

	for (it = Inputs.Begin(); it != Inputs.End(); it++)
	{
		const std::string function = (*it)["Function"].GetString();

		InputFunction inputFunction = InputFunction(function);

		const rapidjson::Value& keys = (*it)["Input"];

		rapidjson::Value::ConstValueIterator key;

		for (key = keys.Begin(); key != keys.End(); key++)
		{
			const std::string inputName = key->GetString();
			const GameInput gameInput = InputConverter::StringToEnum(inputName);
			inputFunction.AddInput(gameInput);
		}

		playerInputs->push_back(inputFunction);
	}

	return playerInputs;
}

void LevelLoader::LoadSystems(const rapidjson::Value& Systems)
{
	SystemManager * systemManager = SystemManager::Instance();

	if (Systems.HasMember("Render"))
	{
		std::vector<iSystem *> renderSystems = CreateSystems(Systems["Render"]);

		for (int i = 0; i < renderSystems.size(); i++)
		{
			systemManager->AddRenderSystem(renderSystems.at(i));
		}
	}

	if (Systems.HasMember("Update"))
	{
		std::vector<iSystem *> updateSystems = CreateSystems(Systems["Update"]);

		for (int i = 0; i < updateSystems.size(); i++)
		{
			systemManager->AddUpdateSystem(updateSystems.at(i));
		}
	}
}

std::vector<iSystem *> LevelLoader::CreateSystems(const rapidjson::Value& pSystems)
{
	std::vector<iSystem *> systems;

	rapidjson::Value::ConstValueIterator it;

	for (it = pSystems.Begin(); it != pSystems.End(); it++)
	{
		const std::string system = (*it)["System"].GetString();

		if (system == "RiggedAnimation")
		{
			systems.push_back(new RiggedAnimationSystem());
		}
		else if (system == "Shadow")
		{
			glm::vec3 topLeftCoord = glm::vec3(0);

			const rapidjson::Value& vec1 = (*it)["TopLeftCoord"];

			for (rapidjson::SizeType i = 0; i < vec1.Size(); i++)
			{
				topLeftCoord[i] = vec1[i].GetFloat();
			}

			glm::vec3 bottomRightCoord = glm::vec3(0);

			const rapidjson::Value& vec2 = (*it)["BottomRightCoord"];

			for (rapidjson::SizeType i = 0; i < vec2.Size(); i++)
			{
				bottomRightCoord[i] = vec2[i].GetFloat();
			}

			systems.push_back(new ShadowSystem(topLeftCoord, bottomRightCoord));
		}
		else if (system == "Render")
		{
			systems.push_back(new RenderSystem());
		}
		else if (system == "Input")
		{
			systems.push_back(new InputSystem());
		}
		else if (system == "ArtificialIntelligence")
		{
			systems.push_back(new ArtificialIntelligenceSystem());
		}
		else if (system == "Physics")
		{
			systems.push_back(new PhysicsSystem());
		}
		else if (system == "Audio")
		{
			systems.push_back(new AudioSystem());
		}
	}

	return systems;
}

int LevelLoader::GetLine(std::string & file, const int location)
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

std::map<std::string, const rapidjson::Value&> LevelLoader::templates;