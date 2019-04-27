#include "irrKlangEngine.h"

#include "irrKlangBuffer.h"

irrklang::ISoundEngine * irrKlangEngine::engine = nullptr;

irrKlangEngine::irrKlangEngine()
{
	engine = irrklang::createIrrKlangDevice();
}


irrKlangEngine::~irrKlangEngine()
{
	engine->drop();
}

//Creates a buffer from the audio file
Buffer * irrKlangEngine::GenerateBuffer(const std::string & fileName)
{
	Buffer * buffer = new irrKlangBuffer(engine->addSoundSourceFromFile(fileName.c_str()));
	return buffer;
}

//Sets the listener position
void irrKlangEngine::SetListenerPosition(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up)
{
	engine->setListenerPosition(irrklang::vec3df(position.x, position.y, position.z),
		irrklang::vec3df(lookAt.x, lookAt.y, lookAt.z),
		irrklang::vec3df(0, 0, 0), irrklang::vec3df(up.x, up.y, up.z));
}

//Deletes source from the engine
void irrKlangEngine::DeleteSource(Source * const pSource) const
{
	delete pSource;
}


//Deletes buffer from the engine
void irrKlangEngine::DeleteBuffer(Buffer * const pBuffer) const
{
	delete pBuffer;
}

//Removes all sounds
void irrKlangEngine::Clear()
{
	engine->stopAllSounds();
}

//Pauses all sounds
void irrKlangEngine::PauseSounds(bool pauseState)
{
	engine->setAllSoundsPaused(pauseState);
}