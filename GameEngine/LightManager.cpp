#include "OpenGL.h"
#include "LightManager.h"
#include "glm/gtc/type_ptr.hpp"
#include "OpenGL.h"
#include <string>
#include <algorithm>
#include "RenderManager.h"

LightManager * LightManager::instance = nullptr;

LightManager::LightManager() : directional(nullptr) 
{
	directionalShadowTexture = RenderManager::Instance()->CreateDirectionShadowFrameBuffer(shadowWidth, shadowHeight);

	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		FrameBuffer * const shadow = RenderManager::Instance()->CreatePointShadowFrameBuffer(shadowWidth, shadowHeight);
		
		pointShadowTexture.push_back(shadow);
	}
}

LightManager::~LightManager()
{
	Clear();

	for (int i = 0; i < pointShadowTexture.size(); i++)
	{
		delete pointShadowTexture.at(i);
		pointShadowTexture.at(i) = nullptr;
	}

	pointShadowTexture.clear();

	delete directional;
}

void LightManager::AddPointLight(const glm::vec3 & pLocation,const glm::vec3 & pLightColour, const float pAttenuation)
{
	PointLight * const pointLight = new PointLight();
	pointLight->location = pLocation;
	pointLight->lightColour = pLightColour;
	pointLight->attenuation = pAttenuation;
	pointLights.push_back(pointLight);
}

void LightManager::RemovePointLight(PointLight * const pPointLight)
{
	const std::vector<PointLight *>::iterator iterator = find(pointLights.begin(), pointLights.end(), pPointLight);

	if (iterator != pointLights.end())
	{
		pointLights.erase(iterator);
	}
}

void LightManager::AddSpotLight(const glm::vec3 & pLocation, const glm::vec3 & pDirection, const glm::vec3 & pLightColour, const float pAngleInner, const float pAngleOutside)
{
	SpotLight * const spotLight = new SpotLight();
	spotLight->location = pLocation;
	spotLight->direction = pDirection;
	spotLight->lightColour = pLightColour;
	spotLight->angleInner = pAngleInner;
	spotLight->angleOutside = pAngleOutside;
	spotLights.push_back(spotLight);
}

void LightManager::RemoveSpotLight(SpotLight * const pSpotLight)
{
	const std::vector<SpotLight *>::iterator iterator = find(spotLights.begin(), spotLights.end(), pSpotLight);

	if (iterator != spotLights.end())
	{
		spotLights.erase(iterator);
	}
}

void LightManager::Update(const glm::vec3 & pViewLocation)
{
	renderPointLights.clear();
	renderSpotLights.clear();

	for (int i = 0; i < pointLights.size(); i++)
	{
		if (renderPointLights.size() < MAX_LIGHTS)
		{
			renderPointLights.push_back(pointLights.at(i));
		}
		else
		{
			const float checkDistance = glm::distance(pointLights.at(i)->location, pViewLocation);
			float maxDistance = 0.0f;
			float location = 0;

			for (int j = 0; j < renderPointLights.size(); j++)
			{
				const float possDistance = glm::distance(renderPointLights.at(j)->location, pViewLocation);

				if (checkDistance < possDistance && possDistance > maxDistance)
				{
					maxDistance = possDistance;
					location = j;
				}
			}

			if (maxDistance > checkDistance)
			{
				renderPointLights.at(location) = pointLights.at(i);
			}
		}
	}

	for (int i = 0; i < spotLights.size(); i++)
	{
		if ((renderPointLights.size() + renderSpotLights.size()) < MAX_LIGHTS)
		{
			renderSpotLights.push_back(spotLights.at(i));
		}
		else
		{
			const float checkDistance = glm::distance(spotLights.at(i)->location, pViewLocation);
			float maxDistance = 0.0f;
			float location = 0;

			for (int j = 0; j < renderPointLights.size(); j++)
			{
				const float possDistance = glm::distance(renderPointLights.at(j)->location, pViewLocation);

				if (checkDistance < possDistance && possDistance > maxDistance)
				{
					maxDistance = possDistance;
					location = j;
				}
			}

			for (int j = 0; j < renderSpotLights.size(); j++)
			{
				const float possDistance = glm::distance(renderSpotLights.at(j)->location, pViewLocation);

				if (checkDistance < possDistance && possDistance > maxDistance)
				{
					maxDistance = possDistance;
					location = j + 20;
				}
			}

			if (maxDistance > checkDistance)
			{
				if (location < 20)
				{
					renderSpotLights.push_back(spotLights.at(i));
					renderPointLights.erase(renderPointLights.begin() + location);
				}
				else
				{
					renderSpotLights.at(location - 20) = spotLights.at(i);
				}
			}
		}
	}

	int light = 0;

	for (int i = 0; i < renderPointLights.size(); i++)
	{
		renderPointLights[i]->shadowTexture = pointShadowTexture[light];
		light++;
	}
	
	for (int i = 0; i < renderSpotLights.size(); i++)
	{
		renderSpotLights[i]->shadowTexture = pointShadowTexture[light];
		light++;
	}
}

void LightManager::Render(Shader * const pShader)
{
	RenderManager * const renderManager = RenderManager::Instance();

	if (directional != nullptr)
	{
		renderManager->SetUniform1i(pShader, "DirectionLightUsed", true);

		renderManager->SetUniform3fv(pShader, "DirectionLightDirection", directional->direction);
		renderManager->SetUniform3fv(pShader, "DirectionLightColour", directional->lightColour);
		renderManager->SetUniformMatrix4fv(pShader, "dirLightPerspective", directional->perspective, false);
		renderManager->SetUniformMatrix4fv(pShader, "dirLightView", directional->view, false);
		renderManager->UseFrameBufferTexture(pShader, "DirLightShadow", directionalShadowTexture, 2);
	}
	else
	{
		renderManager->SetUniform1i(pShader, "DirectionLightUsed", false);
	}

	renderManager->SetUniform1i(pShader, "TotalPointLights", renderPointLights.size());

	unsigned int currentLight = 0;

	for (int i = 0; i < renderPointLights.size(); i++)
	{
		renderManager->SetUniform3fv(pShader, "pointLights[" + std::to_string(i) + "].location", renderPointLights.at(i)->location);
		renderManager->SetUniform3fv(pShader, "pointLights[" + std::to_string(i) + "].lightColour", renderPointLights.at(i)->lightColour);
		renderManager->SetUniform1f(pShader, "pointLights[" + std::to_string(i) + "].attenuation", renderPointLights.at(i)->attenuation);
		renderManager->SetUniform1f(pShader, "pointLights[" + std::to_string(i) + "].farDepth", renderPointLights.at(i)->farPlane);
		renderManager->UseFrameBufferTexture(pShader, "depthMaps[" + std::to_string(currentLight) + "]", renderPointLights.at(i)->shadowTexture, 3 + currentLight);
		currentLight++;
	}

	renderManager->SetUniform1i(pShader, "TotalSpotLights", renderSpotLights.size());

	for (int i = 0; i < renderSpotLights.size(); i++)
	{
		renderManager->SetUniform3fv(pShader, "spotLights[" + std::to_string(i) + "].location", renderSpotLights.at(i)->location);
		renderManager->SetUniform3fv(pShader, "spotLights[" + std::to_string(i) + "].lightColour", renderSpotLights.at(i)->lightColour);
		renderManager->SetUniform3fv(pShader, "spotLights[" + std::to_string(i) + "].direction", renderSpotLights.at(i)->direction);
		renderManager->SetUniform1f(pShader, "spotLights[" + std::to_string(i) + "].angleInner", cos(glm::radians(renderSpotLights.at(i)->angleInner)));
		renderManager->SetUniform1f(pShader, "spotLights[" + std::to_string(i) + "].angleOuter", cos(glm::radians(renderSpotLights.at(i)->angleOutside)));
		renderManager->SetUniform1f(pShader, "spotLights[" + std::to_string(i) + "].farPlane", renderSpotLights.at(i)->farPlane);
		renderManager->UseFrameBufferTexture(pShader, "depthMaps[" + std::to_string(currentLight) + "]", renderSpotLights.at(i)->shadowTexture, 3 + currentLight);
		currentLight++;
	}
}

void LightManager::Clear()
{
	renderSpotLights.clear();
	renderPointLights.clear();

	for (int i = 0; i < spotLights.size(); i++)
	{
		delete spotLights.at(i);
		spotLights.at(i) = nullptr;
	}

	spotLights.clear();

	for (int i = 0; i < pointLights.size(); i++)
	{
		delete pointLights.at(i);
		pointLights.at(i) = nullptr;
	}

	pointLights.clear();
}