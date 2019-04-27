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

//Updates the source according to values in the audiocomponent
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

//Makes sure all audio clips managed by audio manager are updated
void AudioManager::Update()
{
	Camera * const camera = CameraManager::Instance()->GetCamera();

	const glm::vec3 pos = camera->GetPosition();
	const glm::vec3 up = camera->GetUp();
	const glm::vec3 lookAt = camera->GetLookAt();

	audioEngine->SetListenerPosition(pos, lookAt, up);

	//Keeps the sounds next to the camera
	for (int i = 0; i < cameraSounds.size(); i++)
	{
		Source * const source = cameraSounds.at(i);

		//If finished remove sound
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
		
		//If finished remove sound
		if (source->IsFinished())
		{
			DeleteSource(source);
			delete locationSounds.at(i);
			locationSounds.erase(locationSounds.begin() + i);
			i--;
			continue;
		}
	}

	//Keeps the sounds next to the entity
	for (int i = 0; i < entitySounds.size(); i++)
	{
		Source * const source = entitySounds.at(i)->GetSource();

		//If finished remove sound
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

//Plays sound next at the camera location
void AudioManager::PlayAudio(const std::string & sound)
{
	Source * const source = ResourceManager::GetAudio(sound);

	source->SetIsPaused(false);

	Camera * const camera = CameraManager::Instance()->GetCamera();

	const glm::vec3 pos = camera->GetPosition();

	source->SetPosition(pos);

	cameraSounds.push_back(source);
}

//Plays sound at given location
void AudioManager::PlayAudioAtLocation(const std::string & sound, const glm::vec3 & location)
{
	Source * const source = ResourceManager::GetAudio(sound);

	source->SetIsPaused(false);

	source->SetPosition(location);

	LocationSound * const locSound = new LocationSound(source, location);

	locationSounds.push_back(locSound);
}

//Plays sound at the entity location
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

