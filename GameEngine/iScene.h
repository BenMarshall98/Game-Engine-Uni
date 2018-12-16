#pragma once
class iScene
{
public:
	iScene();
	virtual ~iScene();

	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void Load() = 0;
	virtual void Close() = 0;

	virtual void Resize(int width, int height) {};
};

