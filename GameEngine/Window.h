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

	//Gets the width of window
	static int GetWidth()
	{
		return width;
	}

	//Gets the height of window
	static int GetHeight()
	{
		return height;
	}

	//Sets the width
	static void InitWidth(int pWidth)
	{
		width = pWidth;
	}

	//Sets the height
	static void InitHeight(int pHeight)
	{
		height = pHeight;
	}

	//Sets if winodw is fullscreen
	static void InitFullScreen(bool pFullscreen)
	{
		fullScreen = pFullscreen;
	}

	//Sets the framerate of window
	static void SetFrameRate(int pFrameRate)
	{
		FPS = pFrameRate;
	}

	//Gets the framerate of window
	static int GetFrameRate()
	{
		return FPS;
	}

	//Checks if window is fullscreen
	static bool IsFullScreen()
	{
		return fullScreen;
	}
};

