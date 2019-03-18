#pragma once
class Window
{
protected:
	static int width;
	static int height;

public:
	Window();
	virtual ~Window();

	virtual void Load() = 0;
	virtual void LimitFPS(float FPS) = 0;
	virtual void WindowEvents() = 0;
	virtual bool IsRunning() = 0;
	virtual void ChangeSize(float width, float height) = 0;
	virtual void Fullscreen() = 0;

	static int GetWidth()
	{
		return width;
	}

	static int GetHeight()
	{
		return height;
	}
};

