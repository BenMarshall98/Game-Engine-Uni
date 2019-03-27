#pragma once

#pragma comment(lib, "../irrKlang-64bit-1.6.0/lib/Winx64-visualStudio/irrKlang.lib")
#include "irrKlang-64bit-1.6.0/include/irrKlang.h"

#include "Source.h"

class irrKlangSource : public Source
{
private:
	irrklang::ISound * source;

public:
	irrKlangSource(irrklang::ISound * pSource);
	~irrKlangSource();

	void SetPosition(glm::vec3 position) override;
	void SetIsPaused(bool paused) override;
	void SetPlayPosition(int position) override;
	void SetIsLooped(bool looped) override;
	bool GetIsPaused() override;
	bool IsFinished() override;
};

