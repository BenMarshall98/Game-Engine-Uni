#include "OpenGL.h"
#include "LightManager.h"
#include "glm/gtc/type_ptr.hpp"
#include <string>
#include <algorithm>

LightManager * LightManager::instance = nullptr;

void LightManager::SetDirectionalLight(vec3 direction, vec3 lightColour)
{
	delete directional;
	directional = new Directional();
	directional->direction = direction;
	directional->lightColour = lightColour;
}

void LightManager::SetDirectionalLight(Directional * pDirectional)
{
	delete directional;
	directional = pDirectional;
}

void LightManager::AddPointLight(PointLight * pPointLight)
{
	pointLights.push_back(pPointLight);
}

void LightManager::AddPointLight(vec3 pLocation, vec3 pLightColour, float pAttenuation)
{
	PointLight * pointLight = new PointLight();
	pointLight->location = pLocation;
	pointLight->lightColour = pLightColour;
	pointLight->attenuation = pAttenuation;
	pointLights.push_back(pointLight);
}

void LightManager::RemovePointLight(PointLight * pPointLight)
{
	vector<PointLight *>::iterator iterator = find(pointLights.begin(), pointLights.end(), pPointLight);

	if (iterator != pointLights.end())
	{
		pointLights.erase(iterator);
	}
}

void LightManager::AddSpotLight(SpotLight * pSpotLight)
{
	spotLights.push_back(pSpotLight);
}

void LightManager::AddSpotLight(vec3 pLocation, vec3 pDirection, vec3 pLightColour, float pAngleInner, float pAngleOutside)
{
	SpotLight * spotLight = new SpotLight();
	spotLight->location = pLocation;
	spotLight->direction = pDirection;
	spotLight->lightColour = pLightColour;
	spotLight->angleInner = pAngleInner;
	spotLight->angleOutside = pAngleOutside;
	spotLights.push_back(spotLight);
}

void LightManager::RemoveSpotLight(SpotLight * pSpotLight)
{
	vector<SpotLight *>::iterator iterator = find(spotLights.begin(), spotLights.end(), pSpotLight);

	if (iterator != spotLights.end())
	{
		spotLights.erase(iterator);
	}
}

void LightManager::Update(vec3 pViewLocation)
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
			float checkDistance = distance(pointLights.at(i)->location, pViewLocation);
			float maxDistance = 0.0f;
			float location = 0;

			for (int j = 0; j < renderPointLights.size(); j++)
			{
				float possDistance = distance(renderPointLights.at(j)->location, pViewLocation);

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
			float checkDistance = distance(spotLights.at(i)->location, pViewLocation);
			float maxDistance = 0.0f;
			float location = 0;

			for (int j = 0; j < renderPointLights.size(); j++)
			{
				float possDistance = distance(renderPointLights.at(j)->location, pViewLocation);

				if (checkDistance < possDistance && possDistance > maxDistance)
				{
					maxDistance = possDistance;
					location = j;
				}
			}

			for (int j = 0; j < renderSpotLights.size(); j++)
			{
				float possDistance = distance(renderSpotLights.at(j)->location, pViewLocation);

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
}

void LightManager::Render(int ShaderID)
{
	int useDirectionLight = glGetUniformLocation(ShaderID, "DirectionLightUsed");

	if (directional != nullptr)
	{
		glUniform1i(useDirectionLight, GL_TRUE);

		int directionalDirection = glGetUniformLocation(ShaderID, "DirectionLightDirection");
		glUniform3fv(directionalDirection, 1, value_ptr(directional->direction));

		int directionalLightColour = glGetUniformLocation(ShaderID, "DirectionLightColour");
		glUniform3fv(directionalLightColour, 1, value_ptr(directional->lightColour));
	}
	else
	{
		glUniform1i(useDirectionLight, GL_FALSE);
	}

	int totalPointLight = glGetUniformLocation(ShaderID, "TotalPointLights");
	glUniform1i(totalPointLight, renderPointLights.size());

	for (int i = 0; i < renderPointLights.size(); i++)
	{
		int PointLightLocation = glGetUniformLocation(ShaderID, ("pointLights[" + to_string(i) + "].location").c_str());
		glUniform3fv(PointLightLocation, 1, value_ptr(renderPointLights.at(i)->location));

		int PointLightColour = glGetUniformLocation(ShaderID, ("pointLights[" + to_string(i) + "].lightColour").c_str());
		glUniform3fv(PointLightColour, 1, value_ptr(renderPointLights.at(i)->lightColour));

		int PointLightAttenuation = glGetUniformLocation(ShaderID, ("pointLights[" + to_string(i) + "].attenuation").c_str());
		glUniform1f(PointLightAttenuation, renderPointLights.at(i)->attenuation);
	}

	int totalSpotLight = glGetUniformLocation(ShaderID, "TotalSpotLights");
	glUniform1i(totalSpotLight, renderSpotLights.size());

	for (int i = 0; i < renderSpotLights.size(); i++)
	{
		int SpotLightLocation = glGetUniformLocation(ShaderID, ("spotLights[" + to_string(i) + "].location").c_str());
		glUniform3fv(SpotLightLocation, 1, value_ptr(renderSpotLights.at(i)->location));

		int SpotLightColour = glGetUniformLocation(ShaderID, ("spotLights[" + to_string(i) + "].lightColour").c_str());
		glUniform3fv(SpotLightColour, 1, value_ptr(renderSpotLights.at(i)->lightColour));

		int SpotLightDirection = glGetUniformLocation(ShaderID, ("spotLights[" + to_string(i) + "].direction").c_str());
		glUniform3fv(SpotLightDirection, 1, value_ptr(renderSpotLights.at(i)->direction));

		int SpotLightAngleInner = glGetUniformLocation(ShaderID, ("spotLights[" + to_string(i) + "].angleInner").c_str());
		glUniform1f(SpotLightAngleInner, cos(radians(renderSpotLights.at(i)->angleInner)));

		int SpotLightAngleOuter = glGetUniformLocation(ShaderID, ("spotLights[" + to_string(i) + "].angleOuter").c_str());
		glUniform1f(SpotLightAngleOuter, cos(radians(renderSpotLights.at(i)->angleOutside)));
	}
}