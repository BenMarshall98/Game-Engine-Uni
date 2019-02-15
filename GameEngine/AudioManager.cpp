#include "AudioManager.h"
#include "CameraManager.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "ComponentPosition.h"
#include <fstream>

AudioManager * AudioManager::instance = nullptr;

AudioManager::AudioManager()
{
	engine = createIrrKlangDevice();
}


AudioManager::~AudioManager()
{
	engine->drop();
}

void * AudioManager::GenerateBuffer(const string & fileName)
{
	ISoundSource * const buffer = engine->addSoundSourceFromFile(fileName.c_str());

	return buffer;
}

void * AudioManager::GenerateSource(void * const pBuffer)
{
	ISoundSource * const mBuffer = static_cast<ISoundSource *>(pBuffer);
	ISound * const source = engine->play3D(mBuffer, vec3df(0, 0, 0), false, true, true);

	return source;
}

void AudioManager::UpdateComponentSound(void * const pSource, const vec3 & position, const AudioPlayback playback) const
{
	ISound * const mSource = static_cast<ISound *>(pSource);

	if (!mSource)
	{
		return;
	}
	
	mSource->setPosition(vec3df(position.x, position.y, position.z));

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

	const vec3 pos = camera->GetPosition();
	const vec3 up = camera->GetUp();
	const vec3 lookAt = camera->GetLookAt();

	engine->setListenerPosition(vec3df(pos.x, pos.y, pos.z), vec3df(lookAt.x, lookAt.y, lookAt.z),
		vec3df(0, 0, 0), vec3df(up.x, up.y, up.z));

	for (int i = 0; i < cameraSounds.size(); i++)
	{
		ISound * const source = static_cast<ISound *>(cameraSounds.at(i));

		if (source->isFinished())
		{
			DeleteSource(source);
			cameraSounds.erase(cameraSounds.begin() + i);
			i--;
			continue;
		}

		source->setPosition(vec3df(pos.x, pos.y, pos.z));
	}

	for (int i = 0; i < locationSounds.size(); i++)
	{
		ISound * const source = static_cast<ISound *>(locationSounds.at(i)->GetSource());
		
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
		ISound * const source = static_cast<ISound *>(entitySounds.at(i)->GetSource());

		if (source->isFinished())
		{
			DeleteSource(source);
			delete entitySounds.at(i);
			entitySounds.erase(entitySounds.begin() + i);
			i--;
			continue;
		}

		iComponent * const componentPosition = EntityManager::Instance()->GetComponentOfEntity(entitySounds.at(i)->GetEntity(), ComponentType::COMPONENT_POSITION);

		const vec3 entPos = dynamic_cast<ComponentPosition *>(componentPosition)->GetUpdatePosition();

		source->setPosition(vec3df(entPos.x, entPos.y, entPos.z));
	}
}

void AudioManager::PlayAudio(const string & sound)
{
	void * const preSource = ResourceManager::GetAudio(sound);

	ISound * const source = static_cast<ISound *>(preSource);

	source->setIsPaused(false);

	Camera * const camera = CameraManager::Instance()->GetCamera();

	const vec3 pos = camera->GetPosition();

	source->setPosition(vec3df(pos.x, pos.y, pos.z));

	cameraSounds.push_back(source);
}

void AudioManager::PlayAudioAtLocation(const string & sound, const vec3 & location)
{
	void * const preSource = ResourceManager::GetAudio(sound);

	ISound * const source = static_cast<ISound *>(preSource);

	source->setIsPaused(false);

	source->setPosition(vec3df(location.x, location.y, location.z));

	LocationSound * const locSound = new LocationSound(source, location);

	locationSounds.push_back(locSound);
}

void AudioManager::PlayAudioAtEntityLocation(const string & sound, Entity * const entity)
{
	void * const preSource = ResourceManager::GetAudio(sound);

	ISound * const source = static_cast<ISound *>(preSource);
	
	source->setIsPaused(false);

	iComponent * const componentPosition = EntityManager::Instance()->GetComponentOfEntity(entity, ComponentType::COMPONENT_POSITION);

	const vec3 pos = dynamic_cast<ComponentPosition * >(componentPosition)->GetUpdatePosition();

	source->setPosition(vec3df(pos.x, pos.y, pos.z));

	EntitySound * const entSound = new EntitySound(source, entity);
	
	entitySounds.push_back(entSound);
}

void AudioManager::DeleteSource(void * const pSource) const
{
	ISound * const mSource = static_cast<ISound *>(pSource);
	mSource->stop();
	mSource->drop();
}

void AudioManager::DeleteBuffer(void * const pBuffer) const
{
	ISoundSource * const mBuffer = static_cast<ISoundSource *>(pBuffer);
	mBuffer->drop();
}