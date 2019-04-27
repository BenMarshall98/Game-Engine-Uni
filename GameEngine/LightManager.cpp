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

//Adds a point light
void LightManager::AddPointLight(const glm::vec3 & pLocation,const glm::vec3 & pLightColour, const float pAttenuation)
{
	PointLight * const pointLight = new PointLight();
	pointLight->location = pLocation;
	pointLight->lightColour = pLightColour;
	pointLight->attenuation = pAttenuation;
	pointLights.push_back(pointLight);
}

//Removes point light
void LightManager::RemovePointLight(PointLight * const pPointLight)
{
	const std::vector<PointLight *>::iterator iterator = find(pointLights.begin(), pointLights.end(), pPointLight);

	if (iterator != pointLights.end())
	{
		pointLights.erase(iterator);
	}
}

//Adds a spot light
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

//Removes spot light
void LightManager::RemoveSpotLight(SpotLight * const pSpotLight)
{
	const std::vector<SpotLight *>::iterator iterator = find(spotLights.begin(), spotLights.end(), pSpotLight);

	if (iterator != spotLights.end())
	{
		spotLights.erase(iterator);
	}
}

//Gets the lights closest to the camera
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

//Renders the closest lights to the camera
void LightManager::Render(Shader * const pShader)
{
	RenderManager * const renderManager = RenderManager::Instance();

	if (directional != nullptr)
	{
		UniformLocation * dirLightUsedLocation = renderManager->GetUniformLocation(pShader, "DirectionLightUsed");
		renderManager->SetUniform1i(dirLightUsedLocation, true);

		UniformLocation * dirLightDirectionLocation = renderManager->GetUniformLocation(pShader, "DirectionLightDirection");
		renderManager->SetUniform3fv(dirLightDirectionLocation, directional->direction);

		UniformLocation * dirLightColourLocation = renderManager->GetUniformLocation(pShader, "DirectionLightColour");
		renderManager->SetUniform3fv(dirLightColourLocation, directional->lightColour);

		UniformLocation * dirLightPerspectiveLocation = renderManager->GetUniformLocation(pShader, "dirLightPerspective");
		renderManager->SetUniformMatrix4fv(dirLightPerspectiveLocation, directional->perspective, false);

		UniformLocation * dirLightViewLocation = renderManager->GetUniformLocation(pShader, "dirLightView");
		renderManager->SetUniformMatrix4fv(dirLightViewLocation, directional->view, false);

		UniformLocation * dirLightShadowLocation = renderManager->GetUniformLocation(pShader, "DirLightShadow");
		renderManager->UseFrameBufferTexture(dirLightShadowLocation, directionalShadowTexture, 2);

		delete dirLightUsedLocation;
		delete dirLightDirectionLocation;
		delete dirLightColourLocation;
		delete dirLightPerspectiveLocation;
		delete dirLightViewLocation;
		delete dirLightShadowLocation;
	}
	else
	{
		UniformLocation * dirLightUsedLocation = renderManager->GetUniformLocation(pShader, "DirectionLightUsed");
		renderManager->SetUniform1i(dirLightUsedLocation, false);

		delete dirLightUsedLocation;
	}

	UniformLocation * totalPointLightsLocation = renderManager->GetUniformLocation(pShader, "TotalPointLights");
	renderManager->SetUniform1i(totalPointLightsLocation, renderPointLights.size());

	delete totalPointLightsLocation;

	unsigned int currentLight = 0;

	for (int i = 0; i < renderPointLights.size(); i++)
	{
		UniformLocation * pointLightsLocation = renderManager->GetUniformLocation(pShader, "pointLights[" + std::to_string(i) + "].location");
		renderManager->SetUniform3fv(pointLightsLocation, renderPointLights.at(i)->location);

		UniformLocation * pointLightsColourLocation = renderManager->GetUniformLocation(pShader, "pointLights[" + std::to_string(i) + "].lightColour");
		renderManager->SetUniform3fv(pointLightsColourLocation, renderPointLights.at(i)->lightColour);

		UniformLocation * pointLightAttenuationLocation = renderManager->GetUniformLocation(pShader, "pointLights[" + std::to_string(i) + "].attenuation");
		renderManager->SetUniform1f(pointLightAttenuationLocation, renderPointLights.at(i)->attenuation);

		UniformLocation * pointLightsFarDepthLocation = renderManager->GetUniformLocation(pShader, "pointLights[" + std::to_string(i) + "].farDepth");
		renderManager->SetUniform1f(pointLightsFarDepthLocation, renderPointLights.at(i)->farPlane);

		UniformLocation * depthMapsLocation = renderManager->GetUniformLocation(pShader, "depthMaps[" + std::to_string(currentLight) + "]");
		renderManager->UseFrameBufferTexture(depthMapsLocation, renderPointLights.at(i)->shadowTexture, 3 + currentLight);

		delete pointLightsLocation;
		delete pointLightsColourLocation;
		delete pointLightAttenuationLocation;
		delete pointLightsFarDepthLocation;
		delete depthMapsLocation;

		currentLight++;
	}

	UniformLocation * totalSpotLightsLocation = renderManager->GetUniformLocation(pShader, "TotalSpotLights");
	renderManager->SetUniform1i(totalSpotLightsLocation, renderSpotLights.size());

	for (int i = 0; i < renderSpotLights.size(); i++)
	{
		UniformLocation * spotLightsLocation = renderManager->GetUniformLocation(pShader, "spotLights[" + std::to_string(i) + "].location");
		renderManager->SetUniform3fv(spotLightsLocation, renderSpotLights.at(i)->location);

		UniformLocation * spotLightsColourLocation = renderManager->GetUniformLocation(pShader, "spotLights[" + std::to_string(i) + "].lightColour");
		renderManager->SetUniform3fv(spotLightsColourLocation, renderSpotLights.at(i)->lightColour);

		UniformLocation * spotLightsDirectionLocation = renderManager->GetUniformLocation(pShader, "spotLights[" + std::to_string(i) + "].direction");
		renderManager->SetUniform3fv(spotLightsDirectionLocation, renderSpotLights.at(i)->direction);

		UniformLocation * spotLightsAngleInnerLocation = renderManager->GetUniformLocation(pShader, "spotLights[" + std::to_string(i) + "].angleInner");
		renderManager->SetUniform1f(spotLightsAngleInnerLocation, cos(glm::radians(renderSpotLights.at(i)->angleInner)));

		UniformLocation * spotLightsAngleOuterLocation = renderManager->GetUniformLocation(pShader, "spotLights[" + std::to_string(i) + "].angleOuter");
		renderManager->SetUniform1f(spotLightsAngleOuterLocation, cos(glm::radians(renderSpotLights.at(i)->angleOutside)));

		UniformLocation * spotLightsFarPlaneLocation = renderManager->GetUniformLocation(pShader, "spotLights[" + std::to_string(i) + "].farPlane");
		renderManager->SetUniform1f(spotLightsFarPlaneLocation, renderSpotLights.at(i)->farPlane);

		UniformLocation * depthMapLocation = renderManager->GetUniformLocation(pShader, "depthMaps[" + std::to_string(currentLight) + "]");
		renderManager->UseFrameBufferTexture(depthMapLocation, renderSpotLights.at(i)->shadowTexture, 3 + currentLight);

		delete spotLightsLocation;
		delete spotLightsColourLocation;
		delete spotLightsDirectionLocation;
		delete spotLightsAngleInnerLocation;
		delete spotLightsAngleOuterLocation;
		delete spotLightsFarPlaneLocation;
		delete depthMapLocation;

		currentLight++;
	}
}

//Removes all lights
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