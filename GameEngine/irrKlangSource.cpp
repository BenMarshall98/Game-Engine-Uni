#include "irrKlangSource.h"



irrKlangSource::irrKlangSource(irrklang::ISound * pSource) : source(pSource)
{
}

irrKlangSource::~irrKlangSource()
{
	source->stop();
	source->drop();
}

//Set the position of the source
void irrKlangSource::SetPosition(glm::vec3 position)
{
	source->setPosition(irrklang::vec3df(position.x, position.y, position.z));
}

//Sets if the audio is paused or not
void irrKlangSource::SetIsPaused(bool paused)
{
	source->setIsPaused(paused);
}

//Sets the position of the audio clip (audio clip position, not source location)
void irrKlangSource::SetPlayPosition(int position)
{
	source->setPlayPosition(position);
}

//Sets if the audio should be looped
void irrKlangSource::SetIsLooped(bool looped)
{
	source->setIsLooped(looped);
}

//Gets if the audio is paused or not
bool irrKlangSource::GetIsPaused()
{
	return source->getIsPaused();
}

//Checks if the source is finished
bool irrKlangSource::IsFinished()
{
	return source->isFinished();
}
