#pragma once

#include <string>

class iScene
{
protected:
	std::string fileName;

public:
	iScene(std::string pFileName);
	virtual ~iScene();

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Swap() = 0;

	virtual void Load() = 0;
	virtual void Close() = 0;

	virtual void Resize(int width, int height) = 0;
};

