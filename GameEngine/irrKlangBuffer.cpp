#include "irrKlangBuffer.h"
#include "irrKlangSource.h"
#include "irrKlangEngine.h"

irrKlangBuffer::irrKlangBuffer(irrklang::ISoundSource * pBuffer) : buffer(pBuffer)
{
}


irrKlangBuffer::~irrKlangBuffer()
{
	buffer->drop();
}

//Builds a new source
Source * irrKlangBuffer::GenerateSource()
{
	Source * source = new irrKlangSource(irrKlangEngine::engine->play3D(buffer, irrklang::vec3df(0, 0, 0), false, true, true));

	return source;
}
