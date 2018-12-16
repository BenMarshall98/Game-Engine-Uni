#pragma once

#include "OpenGL.h"
#include "glm/glm.hpp"
#include <vector>

using namespace glm;
using namespace std;

struct Directional
{
	vec3 direction;
	vec3 lightColour;
};

struct PointLight
{
	vec3 location;
	vec3 lightColour;
	float attenuation;
};

struct SpotLight
{
	vec3 location;
	vec3 direction;
	vec3 lightColour;
	float angleInner;
	float angleOutside;
};

class LightManager
{
private:
	static LightManager * instance;
	const int MAX_LIGHTS = 20;
	Directional * directional;
	vector<PointLight *> pointLights;
	vector<SpotLight *> spotLights;

	vector<PointLight *>renderPointLights;
	vector<SpotLight *>renderSpotLights;

	LightManager() : directional(nullptr) {}

public:

	inline static LightManager * Instance()
	{
		if (instance == nullptr)
		{
			instance = new LightManager();
		}
		return instance;
	}

	void SetDirectionalLight(vec3 pDirection, vec3 pLightColour);
	void SetDirectionalLight(Directional * pDirectional);

	void AddPointLight(PointLight * pPointLight);
	void AddPointLight(vec3 pLocation, vec3 pLightColour, float pAttenuation = 0);
	void RemovePointLight(PointLight * pPointLight);

	void AddSpotLight(SpotLight * pSpotLight);
	void AddSpotLight(vec3 pLocation, vec3 pDirection, vec3 pLightColour, float pAngleInner = 30, float pAngleOutside = 60);
	void RemoveSpotLight(SpotLight * pSpotLight);

	void Update(vec3 pViewLocation);
	void Render(int pShaderID);

	~LightManager() {};
};

