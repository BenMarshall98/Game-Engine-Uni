#include "AudioSystem.h"
#include "ComponentAudio.h"
#include "ComponentPosition.h"

AudioSystem::AudioSystem() : iSystem(std::vector<ComponentType>{
	ComponentType::COMPONENT_AUDIO,
	ComponentType::COMPONENT_POSITION
})
{
}

//Play audio for the entity
void AudioSystem::Action(float timePeriod)
{
	EntityManager * const entityManager = EntityManager::Instance();
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * const componentPosition = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_POSITION);
		iComponent * const componentAudio = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_AUDIO);

		const glm::vec3 position = dynamic_cast<ComponentPosition *>(componentPosition)->GetUpdatePosition();
		Source * const source = dynamic_cast<ComponentAudio *>(componentAudio)->GetUpdateAudioSource();
		const AudioPlayback playback = dynamic_cast<ComponentAudio *>(componentAudio)->GetUpdateAudioPlayback();

		Audio(position, source, playback);

		dynamic_cast<ComponentAudio *>(componentAudio)->SetUpdateAudioPlayback(AudioPlayback::NONE);
	}
}

//Plays audio for the entity
void AudioSystem::Audio(const glm::vec3 & position, Source * const source, const AudioPlayback playback)
{
	AudioManager::Instance()->UpdateComponentSound(source, position, playback);
}

AudioSystem::~AudioSystem()
{
}
