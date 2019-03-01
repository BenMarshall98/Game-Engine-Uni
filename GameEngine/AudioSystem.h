#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "iSystem.h"
#include "AudioManager.h"
#include "glm/glm.hpp"

#include <vector>

class AudioSystem : public iSystem
{
private:
	std::vector<Entity *> EntityList;
	EntityManager * entityManager;
public:
	AudioSystem();
	~AudioSystem();

	AudioSystem& operator=(const AudioSystem&) = delete;
	AudioSystem(AudioSystem&) = delete;

	void Action(void) override;
	void RemoveEntity(Entity * pEntity) override;
	static void Audio(const glm::vec3 & position, void * source, AudioPlayback playback);
};

