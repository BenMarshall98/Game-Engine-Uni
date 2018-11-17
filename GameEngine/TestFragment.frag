#version 330 core
struct PointLight 
{
	vec3 location;
	vec3 lightColour;
	float attenuation;
};

struct SpotLight
{
	vec3 location;
	vec3 lightColour;
	vec3 direction;
	float angleInner;
	float angleOuter;
};

const int MAX_LIGHTS = 20;

in vec2 oTex;
in vec3 oPos;
in vec3 oNormal;

uniform sampler2D texture;
uniform vec3 viewPos;

uniform bool DirectionLightUsed;
uniform vec3 DirectionLightDirection;
uniform vec3 DirectionLightColour;

uniform int TotalPointLights;
uniform PointLight pointLights[MAX_LIGHTS];

uniform int TotalSpotLights;
uniform SpotLight spotLights[MAX_LIGHTS];

out vec4 FragColor;

void main()
{
	vec3 lightColour = vec3(0,0,0);

	vec3 norm = normalize(oNormal);
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

		lightColour = ambient + diffuse + specular;
	}

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

			lightColour += ambient + diffuse + specular;
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

			lightColour += ambient + diffuse + specular;
		}
	}

	//lightColour = lightColour / (int(DirectionLightUsed) + TotalPointLights + TotalSpotLights);

	FragColor = vec4(texture(texture, oTex).rgb * lightColour, 1.0);
}