#include "PathFinding.h"
#include "ComponentPosition.h"
#include <fstream>

PathFinding::PathFinding(string & pTarget, string & pFile, vec2 & pTopLeftCoord) : topLeftCoord(pTopLeftCoord), target(EntityManager::Instance()->GetEntityByName(pTarget))
{
	vector<int> line;

	ifstream in;
	in.open(pFile);

	while (!in.eof())
	{
		char letter = in.get();

		if (letter == '\n')
		{
			map.push_back(line);
			line.clear();
		}
		else if (letter == 'w')
		{
			line.push_back(1);
		}
		else
		{
			line.push_back(0);
		}
	}
	line.erase(line.begin() + line.size() - 1);
	map.push_back(line);
}

PathFinding::~PathFinding()
{
}

void PathFinding::CalculatePath(vec3 currentPosition, quat currentDirection, ComponentPhysics * physicsComponent)
{
	iComponent * positionComponent = EntityManager::Instance()->GetComponentOfEntity(target, ComponentType::COMPONENT_POSITION);
	vec3 targetPosition = dynamic_cast<ComponentPosition *>(positionComponent)->GetRenderPosition();

	ivec2 targetMapLoc = CalculateMapLoc(targetPosition);

	if (targetMapLoc.x < 0 || targetMapLoc.y < 0)
	{
		return;
	}
	else if (map.size() <= targetMapLoc.y || map.at(targetMapLoc.y).size() <= targetMapLoc.x)
	{
		return;
	}
	else if (map.at(targetMapLoc.y).at(targetMapLoc.x) == 1)
	{
		return;
	}

	vector<StarNode *> open;
	vector<StarNode *> closed;

	StarNode * start = new StarNode();
	start->position = currentPosition;
	start->travelTo = 0;
	start->travelFrom = abs(currentPosition.x - targetPosition.x) + abs(currentPosition.z - targetPosition.z);
	start->weight = abs(currentPosition.x - targetPosition.x) + abs(currentPosition.z - targetPosition.z);
	start->parent = nullptr;

	open.push_back(start);

	StarNode * found = nullptr;
	while (open.size() > 0 && !found)
	{
		StarNode * current = nullptr;
		int loc = 0;

		for (int i = 0; i < open.size(); i++)
		{
			if (!current)
			{
				current = open.at(i);
				loc = i;
			}
			else if(current->weight > open.at(i)->weight)
			{
				current = open.at(i);
				loc = i;
			}
		}

		open.erase(open.begin() + loc);
		closed.push_back(current);
		
		for (int i = 0; i < directions.size(); i++)
		{
			StarNode * successor = new StarNode();
			successor->parent = current;
			successor->position = current->position;
			successor->position.x += directions.at(i).x;
			successor->position.z += directions.at(i).y;
			successor->travelTo = current->travelTo + 1;
			successor->travelFrom = abs(successor->position.x - targetPosition.x) + abs(successor->position.z - targetPosition.z);
			successor->weight = successor->travelTo + successor->travelFrom;

			ivec2 successorMapLoc = CalculateMapLoc(successor->position);
			
			if (successorMapLoc == targetMapLoc)
			{
				found = successor;
				break;
			}
			else if (map.at(successorMapLoc.y).at(successorMapLoc.x) == 1)
			{
				delete successor;
				continue;
			}

			for (int j = 0; j < open.size(); j++)
			{
				if (successor->weight >= open.at(j)->weight)
				{
					if (successor->position == open.at(j)->position)
					{
						delete successor;
						successor = nullptr;
						break;
					}
				}
			}

			for (int j = 0; j < closed.size() && successor; j++)
			{
				if (successor->weight >= closed.at(j)->weight)
				{
					if (successor->position == closed.at(j)->position)
					{
						delete successor;
						successor = nullptr;
						break;
					}
				}
			}

			if (successor)
			{
				open.push_back(successor);
			}
		}
	}

	while (found->parent->parent)
	{
		found = found->parent;
	}

	vec3 norm = normalize(found->position - currentPosition);

	float disVelocity = (100 * (1.0 / 60.0));
	vec3 velocity = disVelocity * norm;

	physicsComponent->SetUpdateVelocity(velocity);
}

ivec2 PathFinding::CalculateMapLoc(vec3 position)
{
	vec2 mapish = vec2(-position.x - topLeftCoord.x, -position.z + topLeftCoord.y);

	ivec2 value = ivec2(floor(mapish.x), floor(mapish.y));

	return value;
}
