#pragma once

#include "Camera.h"
#include "Projection.h"
#include <map>
#include <string>

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

	//Sets the camera to use
	inline void SetCamera(Camera * const pCamera)
	{
		camera = pCamera;
	}

	//Sets the projection to use
	inline void SetProjection(Projection * const pProjection)
	{
		projection = pProjection;
	}

	//Updates the camera and projection matrix
	inline void Update()
	{
		camera->Update();
		projection->Update();
	}

	//Deletes the camera and projection
	inline void Clear()
	{
		delete camera;
		camera = nullptr;
		delete projection;
		projection = nullptr;
	}

	void Resize(float width, float height);

	//Keeps the camera concurrency complient
	void Swap()
	{
		if (camera)
		{
			camera->Swap();
		}
	}

	//Gets the camera
	inline Camera * GetCamera() const
	{
		return camera;
	}

	//Gets the projection
	inline Projection * GetProjection() const
	{
		return projection;
	}

	~CameraManager() {};
};

