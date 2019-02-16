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

	vec3 norm = normalize(targetPosition - currentPosition);

	float disVelocity = (100 * (1.0 / 60.0));
	vec3 velocity = disVelocity * norm;

	physicsComponent->SetUpdateVelocity(velocity);
}
