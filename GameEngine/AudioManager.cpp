#include "AudioManager.h"
#include "CameraManager.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "ComponentPosition.h"
#include <fstream>

AudioManager * AudioManager::instance = nullptr;
AudioEngine * AudioManager::audioEngine = nullptr;

std::string AudioManager::audioName = "irrKlang";

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
	Clear();
	instance = nullptr;
}

void AudioManager::UpdateComponentSound(Source * const pSource, const glm::vec3 & position, const AudioPlayback playback) const
{
	if (!pSource)
	{
		return;
	}
	
	pSource->SetPosition(position);

	if (playback == AudioPlayback::PAUSE)
	{
		pSource->SetIsPaused();
	}
	else if (playback == AudioPlayback::STOP)
	{
		pSource->SetIsPaused();
		pSource->SetPlayPosition(0);
	}
	else if (playback == AudioPlayback::PLAY)
	{
		if (pSource->GetIsPaused())
		{
			pSource->SetIsPaused(false);
			pSource->SetIsLooped(true);
		}
	}
}

void AudioManager::Update()
{
	Camera * const camera = CameraManager::Instance()->GetCamera();

	const glm::vec3 pos = camera->GetPosition();
	const glm::vec3 up = camera->GetUp();
	const glm::vec3 lookAt = camera->GetLookAt();

	audioEngine->SetListenerPosition(pos, lookAt, up);

	for (int i = 0; i < cameraSounds.size(); i++)
	{
		Source * const source = cameraSounds.at(i);

		if (source->IsFinished())
		{
			DeleteSource(source);
			cameraSounds.erase(cameraSounds.begin() + i);
			i--;
			continue;
		}

		source->SetPosition(pos);
	}

	for (int i = 0; i < locationSounds.size(); i++)
	{
		Source * const source = locationSounds.at(i)->GetSource();
		
		if (source->IsFinished())
		{
			DeleteSource(source);
			delete locationSounds.at(i);
			locationSounds.erase(locationSounds.begin() + i);
			i--;
			continue;
		}
	}

	for (int i = 0; i < entitySounds.size(); i++)
	{
		Source * const source = entitySounds.at(i)->GetSource();

		if (source->IsFinished())
		{
			DeleteSource(source);
			delete entitySounds.at(i);
			entitySounds.erase(entitySounds.begin() + i);
			i--;
			continue;
		}

		iComponent * const componentPosition = EntityManager::Instance()->GetComponentOfEntity(entitySounds.at(i)->GetEntity(), ComponentType::COMPONENT_POSITION);

		const glm::vec3 entPos = dynamic_cast<ComponentPosition *>(componentPosition)->GetUpdatePosition();

		source->SetPosition(entPos);
	}
}

void AudioManager::PlayAudio(const std::string & sound)
{
	Source * const source = ResourceManager::GetAudio(sound);

	source->SetIsPaused(false);

	Camera * const camera = CameraManager::Instance()->GetCamera();

	const glm::vec3 pos = camera->GetPosition();

	source->SetPosition(pos);

	cameraSounds.push_back(source);
}

void AudioManager::PlayAudioAtLocation(const std::string & sound, const glm::vec3 & location)
{
	Source * const source = ResourceManager::GetAudio(sound);

	source->SetIsPaused(false);

	source->SetPosition(location);

	LocationSound * const locSound = new LocationSound(source, location);

	locationSounds.push_back(locSound);
}

void AudioManager::PlayAudioAtEntityLocation(const std::string & sound, Entity * const entity)
{
	Source * const source = ResourceManager::GetAudio(sound);
	
	source->SetIsPaused(false);

	iComponent * const componentPosition = EntityManager::Instance()->GetComponentOfEntity(entity, ComponentType::COMPONENT_POSITION);

	const glm::vec3 pos = dynamic_cast<ComponentPosition * >(componentPosition)->GetUpdatePosition();

	source->SetPosition(pos);

	EntitySound * const entSound = new EntitySound(source, entity);
	
	entitySounds.push_back(entSound);
}

