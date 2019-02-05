#include "AudioSystem.h"
#include "ComponentAudio.h"
#include "ComponentPosition.h"


AudioSystem::AudioSystem()
{
	entityManager = EntityManager::Instance();
	ComponentType componentTypes[] = { ComponentType::COMPONENT_AUDIO, ComponentType::COMPONENT_POSITION };
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, size(componentTypes));
}

void AudioSystem::RemoveEntity(Entity * pEntity)
{
	vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}

void AudioSystem::Action(void)
{
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * componentPosition = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_POSITION);
		iComponent * componentAudio = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_AUDIO);

		vec3 position = dynamic_cast<ComponentPosition *>(componentPosition)->GetUpdatePosition();
		void * source = dynamic_cast<ComponentAudio *>(componentAudio)->GetUpdateAudioSource();
		AudioPlayback playback = dynamic_cast<ComponentAudio *>(componentAudio)->GetUpdateAudioPlayback();

		Audio(position, source, playback);

		dynamic_cast<ComponentAudio *>(componentAudio)->SetUpdateAudioPlayback(AudioPlayback::NONE);
	}
}

void AudioSystem::Audio(const vec3 & position, void * source, AudioPlayback playback)
{
	AudioManager::Instance()->UpdateComponentSound(source, position, playback);
}

AudioSystem::~AudioSystem()
{
}
