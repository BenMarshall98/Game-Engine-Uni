#include "AudioSystem.h"
#include "ComponentAudio.h"
#include "ComponentPosition.h"

AudioSystem::AudioSystem()
{
	entityManager = EntityManager::Instance();
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, std::size(componentTypes));
}

void AudioSystem::RemoveEntity(Entity * const pEntity)
{
	const std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}

void AudioSystem::AddEntity(Entity * const pEntity)
{
	const bool containsComponents = entityManager->CheckEntityHasComponents(pEntity, componentTypes, std::size(componentTypes));
	bool containsEntity = false;

	const std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		containsEntity = true;
	}

	if (containsEntity && !containsComponents)
	{
		EntityList.erase(it);
	}
	else if (!containsEntity && containsComponents)
	{
		EntityList.push_back(pEntity);
	}
}

void AudioSystem::Action(void)
{
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * const componentPosition = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_POSITION);
		iComponent * const componentAudio = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_AUDIO);

		const glm::vec3 position = dynamic_cast<ComponentPosition *>(componentPosition)->GetUpdatePosition();
		void * const source = dynamic_cast<ComponentAudio *>(componentAudio)->GetUpdateAudioSource();
		const AudioPlayback playback = dynamic_cast<ComponentAudio *>(componentAudio)->GetUpdateAudioPlayback();

		Audio(position, source, playback);

		dynamic_cast<ComponentAudio *>(componentAudio)->SetUpdateAudioPlayback(AudioPlayback::NONE);
	}
}

void AudioSystem::Audio(const glm::vec3 & position, void * const source, const AudioPlayback playback)
{
	AudioManager::Instance()->UpdateComponentSound(source, position, playback);
}

AudioSystem::~AudioSystem()
{
}
