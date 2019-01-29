#include "OpenGL.h"
#include "LightManager.h"
#include "glm/gtc/type_ptr.hpp"
#include "OpenGL.h"
#include <string>
#include <algorithm>

LightManager * LightManager::instance = nullptr;

LightManager::LightManager() : directional(nullptr) 
{
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);

	unsigned int Texture;

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	directionalShadowTexture.FrameBuffer = FBO;
	directionalShadowTexture.ShadowTexture = Texture;

	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		unsigned int pointFBO;
		glGenFramebuffers(1, &pointFBO);

		unsigned int pointCubemap;
		glGenTextures(1, &pointCubemap);

		glBindTexture(GL_TEXTURE_CUBE_MAP, pointCubemap);

		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
				shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, pointFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, pointCubemap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		ShadowFrameBuffer * shadow = new ShadowFrameBuffer;
		shadow->FrameBuffer = pointFBO;
		shadow->ShadowTexture = pointCubemap;
		
		pointShadowTexture.push_back(shadow);
	}
}

void LightManager::AddPointLight(const vec3 & pLocation,const vec3 & pLightColour, const float pAttenuation)
{
	PointLight * const pointLight = new PointLight();
	pointLight->location = pLocation;
	pointLight->lightColour = pLightColour;
	pointLight->attenuation = pAttenuation;
	pointLights.push_back(pointLight);
}

void LightManager::RemovePointLight(PointLight * const pPointLight)
{
	const vector<PointLight *>::iterator iterator = find(pointLights.begin(), pointLights.end(), pPointLight);

	if (iterator != pointLights.end())
	{
		pointLights.erase(iterator);
	}
}

void LightManager::AddSpotLight(const vec3 & pLocation, const vec3 & pDirection, const vec3 & pLightColour, const float pAngleInner, const float pAngleOutside)
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
	const vector<SpotLight *>::iterator iterator = find(spotLights.begin(), spotLights.end(), pSpotLight);

	if (iterator != spotLights.end())
	{
		spotLights.erase(iterator);
	}
}

void LightManager::Update(const vec3 & pViewLocation)
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
			const float checkDistance = distance(pointLights.at(i)->location, pViewLocation);
			float maxDistance = 0.0f;
			float location = 0;

			for (int j = 0; j < renderPointLights.size(); j++)
			{
				const float possDistance = distance(renderPointLights.at(j)->location, pViewLocation);

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
			const float checkDistance = distance(spotLights.at(i)->location, pViewLocation);
			float maxDistance = 0.0f;
			float location = 0;

			for (int j = 0; j < renderPointLights.size(); j++)
			{
				const float possDistance = distance(renderPointLights.at(j)->location, pViewLocation);

				if (checkDistance < possDistance && possDistance > maxDistance)
				{
					maxDistance = possDistance;
					location = j;
				}
			}

			for (int j = 0; j < renderSpotLights.size(); j++)
			{
				const float possDistance = distance(renderSpotLights.at(j)->location, pViewLocation);

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

void LightManager::Render(const int pShaderID)
{
	const int useDirectionLight = glGetUniformLocation(pShaderID, "DirectionLightUsed");

	if (directional != nullptr)
	{
		glUniform1i(useDirectionLight, GL_TRUE);

		const int directionalDirection = glGetUniformLocation(pShaderID, "DirectionLightDirection");
		glUniform3fv(directionalDirection, 1, value_ptr(directional->direction));

		const int directionalLightColour = glGetUniformLocation(pShaderID, "DirectionLightColour");
		glUniform3fv(directionalLightColour, 1, value_ptr(directional->lightColour));

		const int directionalLightPerspective = glGetUniformLocation(pShaderID, "dirLightPerspective");
		glUniformMatrix4fv(directionalLightPerspective, 1, GL_FALSE, value_ptr(directional->perspective));

		const int directionalLightView = glGetUniformLocation(pShaderID, "dirLightView");
		glUniformMatrix4fv(directionalLightView, 1, GL_FALSE, value_ptr(directional->view));

		const int directionalLightTexture = glGetUniformLocation(pShaderID, "DirLightShadow");
		glUniform1i(directionalLightTexture, 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, directionalShadowTexture.ShadowTexture);
	}
	else
	{
		glUniform1i(useDirectionLight, GL_FALSE);
	}

	const int totalPointLight = glGetUniformLocation(pShaderID, "TotalPointLights");
	glUniform1i(totalPointLight, renderPointLights.size());

	unsigned int currentLight = 0;

	for (int i = 0; i < renderPointLights.size(); i++)
	{
		const int PointLightLocation = glGetUniformLocation(pShaderID, ("pointLights[" + to_string(i) + "].location").c_str());
		glUniform3fv(PointLightLocation, 1, value_ptr(renderPointLights.at(i)->location));

		const int PointLightColour = glGetUniformLocation(pShaderID, ("pointLights[" + to_string(i) + "].lightColour").c_str());
		glUniform3fv(PointLightColour, 1, value_ptr(renderPointLights.at(i)->lightColour));

		const int PointLightAttenuation = glGetUniformLocation(pShaderID, ("pointLights[" + to_string(i) + "].attenuation").c_str());
		glUniform1f(PointLightAttenuation, renderPointLights.at(i)->attenuation);

		const int PointLightFarDepth = glGetUniformLocation(pShaderID, ("pointLights[" + to_string(i) + "].farDepth").c_str());
		glUniform1f(PointLightFarDepth, renderPointLights.at(i)->farPlane);

		const int directionalLightTexture = glGetUniformLocation(pShaderID, ("depthMaps[" + to_string(currentLight) + "]").c_str());
		glUniform1i(directionalLightTexture, 3 + currentLight);
		glActiveTexture(GL_TEXTURE3 + currentLight);
		currentLight++;
		glBindTexture(GL_TEXTURE_CUBE_MAP, renderPointLights.at(i)->shadowTexture->ShadowTexture);
	}

	const int totalSpotLight = glGetUniformLocation(pShaderID, "TotalSpotLights");
	glUniform1i(totalSpotLight, renderSpotLights.size());

	for (int i = 0; i < renderSpotLights.size(); i++)
	{
		const int SpotLightLocation = glGetUniformLocation(pShaderID, ("spotLights[" + to_string(i) + "].location").c_str());
		glUniform3fv(SpotLightLocation, 1, value_ptr(renderSpotLights.at(i)->location));

		const int SpotLightColour = glGetUniformLocation(pShaderID, ("spotLights[" + to_string(i) + "].lightColour").c_str());
		glUniform3fv(SpotLightColour, 1, value_ptr(renderSpotLights.at(i)->lightColour));

		const int SpotLightDirection = glGetUniformLocation(pShaderID, ("spotLights[" + to_string(i) + "].direction").c_str());
		glUniform3fv(SpotLightDirection, 1, value_ptr(renderSpotLights.at(i)->direction));

		const int SpotLightAngleInner = glGetUniformLocation(pShaderID, ("spotLights[" + to_string(i) + "].angleInner").c_str());
		glUniform1f(SpotLightAngleInner, cos(radians(renderSpotLights.at(i)->angleInner)));

		const int SpotLightAngleOuter = glGetUniformLocation(pShaderID, ("spotLights[" + to_string(i) + "].angleOuter").c_str());
		glUniform1f(SpotLightAngleOuter, cos(radians(renderSpotLights.at(i)->angleOutside)));

		const int SpotLightFarDepth = glGetUniformLocation(pShaderID, ("spotLights[" + to_string(i) + "].farPlane").c_str());
		glUniform1f(SpotLightFarDepth, renderSpotLights.at(i)->farPlane);

		const int directionalLightTexture = glGetUniformLocation(pShaderID, ("depthMaps[" + to_string(currentLight) + "]").c_str());
		glUniform1i(directionalLightTexture, 3 + currentLight);
		glActiveTexture(GL_TEXTURE3 + currentLight);
		currentLight++;
		glBindTexture(GL_TEXTURE_CUBE_MAP, renderSpotLights.at(i)->shadowTexture->ShadowTexture);
	}
}