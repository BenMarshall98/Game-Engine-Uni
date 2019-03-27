#pragma once
class Window
{
protected:
	static int width;
	static int height;
	static bool fullScreen;
	static int FPS;

public:
	Window();
	virtual ~Window();

	virtual void Load() = 0;
	virtual void LimitFPS() = 0;
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

	static void InitWidth(int pWidth)
	{
		width = pWidth;
	}

	static void InitHeight(int pHeight)
	{
		height = pHeight;
	}

	static void InitFullScreen(bool pFullscreen)
	{
		fullScreen = pFullscreen;
	}

	static void SetFrameRate(int pFrameRate)
	{
		FPS = pFrameRate;
	}

	static int GetFrameRate()
	{
		return FPS;
	}
};

