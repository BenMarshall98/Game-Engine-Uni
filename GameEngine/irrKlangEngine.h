#pragma once

#pragma comment(lib, "../External/irrKlang-64bit-1.6.0/lib/Winx64-visualStudio/irrKlang.lib")
#include "irrKlang-64bit-1.6.0/include/irrKlang.h"

#include "AudioEngine.h"
#include "glm/glm.hpp"
#include <vector>

class irrKlangEngine : public AudioEngine
{
private:
	

public:
	static irrklang::ISoundEngine * engine;

	irrKlangEngine();
	~irrKlangEngine();

	Buffer * GenerateBuffer(const std::string & fileName) override;

	void DeleteBuffer(Buffer * const buffer) const override;
	void DeleteSource(Source * const source) const override;

	void Clear() override;

	void SetListenerPosition(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up) override;

	void PauseSounds(bool pauseState) override;
};

