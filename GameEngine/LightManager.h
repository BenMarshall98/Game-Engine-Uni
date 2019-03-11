#pragma once

#include "OpenGL.h"
#include "glm/glm.hpp"
#include <vector>

struct ShadowFrameBuffer
{
	unsigned int FrameBuffer;
	unsigned int ShadowTexture;
};

struct Directional
{
	glm::vec3 direction;
	glm::vec3 lightColour;
	glm::mat4 perspective;
	glm::mat4 view;
};

struct PointLight
{
	PointLight() {};
	PointLight& operator=(const PointLight&) = delete;
	PointLight(PointLight&) = delete;
	glm::vec3 location;
	glm::vec3 lightColour;
	ShadowFrameBuffer * shadowTexture;
	float farPlane;
	float attenuation;
};

struct SpotLight
{
	SpotLight() {};
	SpotLight& operator=(const SpotLight&) = delete;
	SpotLight(SpotLight&) = delete;
	glm::vec3 location;
	glm::vec3 direction;
	glm::vec3 lightColour;
	ShadowFrameBuffer * shadowTexture;
	float farPlane;
	float angleInner;
	float angleOutside;
};

class LightManager
{
private:
	std::vector<SpotLight *> renderSpotLights;
	std::vector<SpotLight *> spotLights;

	std::vector<PointLight *> pointLights;
	std::vector<PointLight *> renderPointLights;

	ShadowFrameBuffer directionalShadowTexture;
	std::vector<ShadowFrameBuffer *> pointShadowTexture;

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

	inline std::vector<SpotLight*> & GetRenderSpotLights()
	{
		return renderSpotLights;
	}

	inline std::vector<PointLight*> & GetRenderPointLights()
	{
		return renderPointLights;
	}

	inline void SetDirectionalLight(const glm::vec3 & pDirection, const glm::vec3 & pLightColour)
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

	inline ShadowFrameBuffer & GetDirectionalFramebuffer()
	{
		return directionalShadowTexture;
	}

	void AddPointLight(const glm::vec3 & pLocation, const glm::vec3 & pLightColour, const float pAttenuation = 0);
	void RemovePointLight(PointLight * const pPointLight);

	inline void AddSpotLight(SpotLight * const pSpotLight)
	{
		spotLights.push_back(pSpotLight);
	}

	void AddSpotLight(const glm::vec3 & pLocation, const glm::vec3 & pDirection, const glm::vec3 & pLightColour, const float pAngleInner = 30, const float pAngleOutside = 60);
	void RemoveSpotLight(SpotLight * const pSpotLight);

	void Update(const glm::vec3 & pViewLocation);
	void Clear();
	void Render(const int pShaderID);

	~LightManager();
};

