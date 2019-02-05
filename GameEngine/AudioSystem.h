#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "iSystem.h"
#include "AudioManager.h"
#include "glm/glm.hpp"

#include <vector>

using namespace std;

class AudioSystem : public iSystem
{
private:
	vector<Entity *> EntityList;
	EntityManager * entityManager;
public:
	AudioSystem();
	~AudioSystem();

	AudioSystem& operator=(const AudioSystem&) = delete;
	AudioSystem(AudioSystem&) = delete;

	void Action(void) override;
	void RemoveEntity(Entity * pEntity) override;
	static void Audio(const vec3 & position, void * source, AudioPlayback playback);
};

