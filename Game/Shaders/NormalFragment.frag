#version 330 core
struct PointLight 
{
	vec3 location;
	vec3 lightColour;
	float attenuation;
	float farPlane;
};

struct SpotLight
{
	vec3 location;
	vec3 lightColour;
	vec3 direction;
	float angleInner;
	float angleOuter;
	float farPlane;
};

const int MAX_LIGHTS = 20;

in vec2 oTex;
in vec3 oPos;
in vec3 oNormal;
in vec4 oDirLightPos;
in mat3 oTBN;

uniform sampler2D texture_map;
uniform sampler2D normalTexture;
uniform vec3 viewPos;

uniform bool DirectionLightUsed;
uniform vec3 DirectionLightDirection;
uniform vec3 DirectionLightColour;
uniform sampler2D DirLightShadow;

uniform int TotalPointLights;
uniform PointLight pointLights[MAX_LIGHTS];

uniform samplerCube depthMaps[MAX_LIGHTS];

uniform int TotalSpotLights;
uniform SpotLight spotLights[MAX_LIGHTS];

out vec4 FragColor;

float DirectionalShadowCalculation(vec4 LightPos, sampler2D shadowTexture, vec3 normal, vec3 lightDir);
float PointShadowCalculation(vec3 lightPos, vec3 fragPos, samplerCube shadowTexture, float farPlane);

void main()
{
	vec3 lightColour = vec3(0,0,0);

	vec3 normal = oNormal;

	vec3 norm = texture(normalTexture, oTex).rgb;
	norm = normalize(norm * 2.0 - 1.0);
	norm = normalize(norm * oTBN);

	vec3 viewDir = normalize(viewPos - oPos);
	float ambientStrength = 0.1;

	if (DirectionLightUsed)
	{
		// ambient
		vec3 ambient = ambientStrength * DirectionLightColour;

		//diffuse
		vec3 lightDir = normalize(-DirectionLightDirection);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = DirectionLightColour * diff;

		//specular
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		vec3 specular = DirectionLightColour * spec;

		float shadow = DirectionalShadowCalculation(oDirLightPos, DirLightShadow, normal, lightDir);
		lightColour = ambient + ((1.0 - shadow) * (diffuse + specular));
	}

	int mapNumber = 0;

	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		if (i < TotalPointLights)
		{
			// ambient
			vec3 ambient = pointLights[i].lightColour * ambientStrength;

			//diffuse
			vec3 lightDir = normalize(pointLights[i].location - oPos);
			float diff = max(dot(norm, lightDir), 0.0);
			vec3 diffuse = pointLights[i].lightColour * diff;

			//specular
			vec3 reflectDir = reflect(-lightDir, norm);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
			vec3 specular = pointLights[i].lightColour * spec;

			//attenuation
			float distance = length(pointLights[i].location - oPos);
			float attenuation = 1.0 / (1.0 + pointLights[i].attenuation * pow(distance, 2));

			ambient *= attenuation;
			diffuse *= attenuation;
			specular *= attenuation;

			float shadow = PointShadowCalculation(pointLights[i].location, oPos, depthMaps[mapNumber], pointLights[i].farPlane);
			mapNumber++;
			lightColour += ambient + ((1.0 - shadow) * diffuse + specular);
		}
	}

	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		if (i < TotalSpotLights)
		{
			// ambient
			vec3 ambient = spotLights[i].lightColour * ambientStrength;

			//diffuse
			vec3 lightDir = normalize(spotLights[i].location - oPos);
			float diff = max(dot(norm, lightDir), 0.0);
			vec3 diffuse = spotLights[i].lightColour * diff;

			// specular
			vec3 reflectDir = reflect(-lightDir, norm);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
			vec3 specular = spotLights[i].lightColour * spec;

			//spotLight
			float theta = dot(lightDir, normalize(-spotLights[i].direction));
			float epsilon = (spotLights[i].angleInner - spotLights[i].angleOuter);
			float intensity = clamp((theta - spotLights[i].angleOuter) / epsilon, 0.0, 1.0);
			diffuse *= intensity;
			specular *= intensity;

			float shadow = PointShadowCalculation(spotLights[i].location, oPos, depthMaps[mapNumber], spotLights[i].farPlane);
			mapNumber++;
			lightColour += ambient + ((1.0 - shadow) * diffuse + specular);
		}
	}

	//lightColour = lightColour / (int(DirectionLightUsed) + TotalPointLights + TotalSpotLights);

	FragColor = vec4(texture(texture_map, oTex).rgb * lightColour, 1.0);
}

float DirectionalShadowCalculation(vec4 LightPos, sampler2D shadowTexture, vec3 normal, vec3 lightDir)
{
	vec3 projCoords = LightPos.xyz / LightPos.w;

	projCoords = projCoords * 0.5 + 0.5;

	if (projCoords.z > 1.0)
	{
		return 0.0;
	}

	float currentDepth = projCoords.z;

	float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.001);

	float shadow = 0.0;
	vec2 texelSize = 0.5 / textureSize(shadowTexture, 0);

	for (int x = -2; x <= 2; x++)
	{
		for (int y = -2; y <= 2; y++)
		{
			float closestDepth = texture(shadowTexture, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;
		}
	}

	shadow = shadow / 25.0;

	return shadow;
}


float PointShadowCalculation(vec3 lightPos, vec3 fragPos, samplerCube shadowTexture, float farPlane)
{
	vec3 fragToLight = fragPos - lightPos;
	
	float closestDepth = texture(shadowTexture, fragToLight).r;
	
	closestDepth *= farPlane;
	
	float currentDepth = length(fragToLight);
	
	float bias = 0.05;
	
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	
	return shadow;
}