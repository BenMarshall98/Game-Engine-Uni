#pragma once

#include "Camera.h"
#include "Projection.h"
#include <map>
#include <string>

using namespace std;

class CameraManager
{
private:
	Camera * camera;
	Projection * projection;

	static CameraManager * instance;

	CameraManager() : camera(nullptr), projection(nullptr) {}

public:
	static CameraManager * Instance()
	{
		if (instance == nullptr)
		{
			instance = new CameraManager();
		}
		return instance;
	}

	CameraManager(const CameraManager&) = delete;
	CameraManager& operator=(const CameraManager&) = delete;

	inline void SetCamera(Camera * pCamera)
	{
		camera = pCamera;
	}

	inline void SetProjection(Projection * pProjection)
	{
		projection = pProjection;
	}

	inline void Update()
	{
		camera->Update();
		projection->Update();
	}

	void Resize(float width, float height);

	void Swap()
	{
		camera->Swap();
	}

	inline Camera * GetCamera()
	{
		return camera;
	}

	inline Projection * GetProjection()
	{
		return projection;
	}

	~CameraManager() {};
};

