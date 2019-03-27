#include "irrKlangSource.h"



irrKlangSource::irrKlangSource(irrklang::ISound * pSource) : source(pSource)
{
}

irrKlangSource::~irrKlangSource()
{
	source->stop();
	source->drop();
}

void irrKlangSource::SetPosition(glm::vec3 position)
{
	source->setPosition(irrklang::vec3df(position.x, position.y, position.z));
}

void irrKlangSource::SetIsPaused(bool paused)
{
	source->setIsPaused(paused);
}

void irrKlangSource::SetPlayPosition(int position)
{
	source->setPlayPosition(position);
}

void irrKlangSource::SetIsLooped(bool looped)
{
	source->setIsLooped(looped);
}

bool irrKlangSource::GetIsPaused()
{
	return source->getIsPaused();
}

bool irrKlangSource::IsFinished()
{
	return source->isFinished();
}
