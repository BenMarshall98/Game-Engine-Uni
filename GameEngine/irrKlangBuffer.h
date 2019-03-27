#pragma once

#pragma comment(lib, "../irrKlang-64bit-1.6.0/lib/Winx64-visualStudio/irrKlang.lib")
#include "irrKlang-64bit-1.6.0/include/irrKlang.h"

#include "Buffer.h"

class irrKlangBuffer : public Buffer
{
private:
	irrklang::ISoundSource * buffer;

public:
	irrKlangBuffer(irrklang::ISoundSource * pBuffer);
	~irrKlangBuffer();

	Source * GenerateSource() override;
};

