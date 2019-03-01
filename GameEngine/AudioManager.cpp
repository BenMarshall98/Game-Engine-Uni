#include "AudioManager.h"
#include "CameraManager.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "ComponentPosition.h"
#include <fstream>

AudioManager * AudioManager::instance = nullptr;

AudioManager::AudioManager()
{
	engine = irrklang::createIrrKlangDevice();
}


AudioManager::~AudioManager()
{
	engine->drop();
}

void * AudioManager::GenerateBuffer(const std::string & fileName)
{
	irrklang::ISoundSource * const buffer = engine->addSoundSourceFromFile(fileName.c_str());

	return buffer;
}

void * AudioManager::GenerateSource(void * const pBuffer)
{
	irrklang::ISoundSource * const mBuffer = static_cast<irrklang::ISoundSource *>(pBuffer);
	irrklang::ISound * const source = engine->play3D(mBuffer, irrklang::vec3df(0, 0, 0), false, true, true);

	return source;
}

void AudioManager::UpdateComponentSound(void * const pSource, const glm::vec3 & position, const AudioPlayback playback) const
{
	irrklang::ISound * const mSource = static_cast<irrklang::ISound *>(pSource);

	if (!mSource)
	{
		return;
	}
	
	mSource->setPosition(irrklang::vec3df(position.x, position.y, position.z));

	if (playback == AudioPlayback::PAUSE)
	{
		mSource->setIsPaused();
	}
	else if (playback == AudioPlayback::STOP)
	{
		mSource->setIsPaused();
		mSource->setPlayPosition(0);
	}
	else if (playback == AudioPlayback::PLAY)
	{
		if (mSource->getIsPaused())
		{
			mSource->setIsPaused(false);
			mSource->setIsLooped(true);
		}
	}
}

void AudioManager::Update()
{
	Camera * const camera = CameraManager::Instance()->GetCamera();

	const glm::vec3 pos = camera->GetPosition();
	const glm::vec3 up = camera->GetUp();
	const glm::vec3 lookAt = camera->GetLookAt();

	engine->setListenerPosition(irrklang::vec3df(pos.x, pos.y, pos.z), irrklang::vec3df(lookAt.x, lookAt.y, lookAt.z),
		irrklang::vec3df(0, 0, 0), irrklang::vec3df(up.x, up.y, up.z));

	for (int i = 0; i < cameraSounds.size(); i++)
	{
		irrklang::ISound * const source = static_cast<irrklang::ISound *>(cameraSounds.at(i));

		if (source->isFinished())
		{
			DeleteSource(source);
			cameraSounds.erase(cameraSounds.begin() + i);
			i--;
			continue;
		}

		source->setPosition(irrklang::vec3df(pos.x, pos.y, pos.z));
	}

	for (int i = 0; i < locationSounds.size(); i++)
	{
		irrklang::ISound * const source = static_cast<irrklang::ISound *>(locationSounds.at(i)->GetSource());
		
		if (source->isFinished())
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
		irrklang::ISound * const source = static_cast<irrklang::ISound *>(entitySounds.at(i)->GetSource());

		if (source->isFinished())
		{
			DeleteSource(source);
			delete entitySounds.at(i);
			entitySounds.erase(entitySounds.begin() + i);
			i--;
			continue;
		}

		iComponent * const componentPosition = EntityManager::Instance()->GetComponentOfEntity(entitySounds.at(i)->GetEntity(), ComponentType::COMPONENT_POSITION);

		const glm::vec3 entPos = dynamic_cast<ComponentPosition *>(componentPosition)->GetUpdatePosition();

		source->setPosition(irrklang::vec3df(entPos.x, entPos.y, entPos.z));
	}
}

void AudioManager::PlayAudio(const std::string & sound)
{
	void * const preSource = ResourceManager::GetAudio(sound);

	irrklang::ISound * const source = static_cast<irrklang::ISound *>(preSource);

	source->setIsPaused(false);

	Camera * const camera = CameraManager::Instance()->GetCamera();

	const glm::vec3 pos = camera->GetPosition();

	source->setPosition(irrklang::vec3df(pos.x, pos.y, pos.z));

	cameraSounds.push_back(source);
}

void AudioManager::PlayAudioAtLocation(const std::string & sound, const glm::vec3 & location)
{
	void * const preSource = ResourceManager::GetAudio(sound);

	irrklang::ISound * const source = static_cast<irrklang::ISound *>(preSource);

	source->setIsPaused(false);

	source->setPosition(irrklang::vec3df(location.x, location.y, location.z));

	LocationSound * const locSound = new LocationSound(source, location);

	locationSounds.push_back(locSound);
}

void AudioManager::PlayAudioAtEntityLocation(const std::string & sound, Entity * const entity)
{
	void * const preSource = ResourceManager::GetAudio(sound);

	irrklang::ISound * const source = static_cast<irrklang::ISound *>(preSource);
	
	source->setIsPaused(false);

	iComponent * const componentPosition = EntityManager::Instance()->GetComponentOfEntity(entity, ComponentType::COMPONENT_POSITION);

	const glm::vec3 pos = dynamic_cast<ComponentPosition * >(componentPosition)->GetUpdatePosition();

	source->setPosition(irrklang::vec3df(pos.x, pos.y, pos.z));

	EntitySound * const entSound = new EntitySound(source, entity);
	
	entitySounds.push_back(entSound);
}

void AudioManager::DeleteSource(void * const pSource) const
{
	irrklang::ISound * const mSource = static_cast<irrklang::ISound *>(pSource);
	mSource->stop();
	mSource->drop();
}

void AudioManager::DeleteBuffer(void * const pBuffer) const
{
	irrklang::ISoundSource * const mBuffer = static_cast<irrklang::ISoundSource *>(pBuffer);
	mBuffer->drop();
}