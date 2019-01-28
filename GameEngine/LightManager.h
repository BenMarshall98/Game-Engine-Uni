#pragma once

#include "OpenGL.h"
#include "glm/glm.hpp"
#include <vector>

using namespace glm;
using namespace std;

struct ShadowFrameBuffer
{
	unsigned int FrameBuffer;
	unsigned int ShadowTexture;
};

struct Directional
{
	vec3 direction;
	vec3 lightColour;
	mat4 perspective;
	mat4 view;
};

struct PointLight
{
	ShadowFrameBuffer * shadowTexture;
	float farPlane;
	vec3 location;
	vec3 lightColour;
	float attenuation;
};

struct SpotLight
{
	ShadowFrameBuffer * shadowTexture;
	float farPlane;
	vec3 location;
	vec3 direction;
	vec3 lightColour;
	float angleInner;
	float angleOutside;
};

class LightManager
{
private:
	vector<SpotLight *> renderSpotLights;
	vector<SpotLight *> spotLights;

	vector<PointLight *> pointLights;
	vector<PointLight *> renderPointLights;

	ShadowFrameBuffer directionalShadowTexture;
	vector<ShadowFrameBuffer *> pointShadowTexture;

	Directional * directional;

	const int MAX_LIGHTS = 20;

	static LightManager * instance;

	LightManager();

public:

	static const unsigned int shadowWidth = 1024, shadowHeight = 1024;

	inline static LightManager * Instance()
	{
		if (instance == nullptr)
		{
			instance = new LightManager();
		}
		return instance;
	}

	LightManager& operator=(const LightManager&) = delete;
	LightManager(LightManager&) = delete;

	inline vector<SpotLight*> & GetRenderSpotLights()
	{
		return renderSpotLights;
	}

	inline vector<PointLight*> & GetRenderPointLights()
	{
		return renderPointLights;
	}

	inline void SetDirectionalLight(const vec3 & pDirection, const vec3 & pLightColour)
	{
		delete directional;
		directional = new Directional();
		directional->direction = pDirection;
		directional->lightColour = pLightColour;
	}

	inline void SetDirectionalLight(Directional * const pDirectional)
	{
		delete directional;
		directional = pDirectional;
	}

	inline void AddPointLight(PointLight * const pPointLight)
	{
		pointLights.push_back(pPointLight);
	}

	inline Directional * GetDirectionalLight()
	{
		return directional;
	}

	inline ShadowFrameBuffer GetDirectionalFramebuffer()
	{
		return directionalShadowTexture;
	}

	void AddPointLight(const vec3 & pLocation, const vec3 & pLightColour, const float pAttenuation = 0);
	void RemovePointLight(PointLight * const pPointLight);

	inline void AddSpotLight(SpotLight * const pSpotLight)
	{
		spotLights.push_back(pSpotLight);
	}

	void AddSpotLight(const vec3 & pLocation, const vec3 & pDirection, const vec3 & pLightColour, const float pAngleInner = 30, const float pAngleOutside = 60);
	void RemoveSpotLight(SpotLight * const pSpotLight);

	void Update(const vec3 & pViewLocation);
	void Render(const int pShaderID);

	~LightManager()
	{
		delete directional;
	};
};

