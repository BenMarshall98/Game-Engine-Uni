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

	CameraManager() {}

public:
	static CameraManager * Instance()
	{
		if (instance == nullptr)
		{
			instance = new CameraManager();
		}
		return instance;
	}

	void SetCamera(Camera * pCamera);
	void SetProjection(Projection * pProjection);

	Camera * GetCamera();
	Projection * GetProjection();

	~CameraManager() {};
};

