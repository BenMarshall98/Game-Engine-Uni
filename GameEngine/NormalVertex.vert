#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec2 oTex;
out vec3 oPos;
out mat3 oTBN;

void main()
{
	oTex = aTex;
	oPos = vec3(model * vec4(aPos, 1.0));

	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 N = normalize(normalMatrix * aNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	mat3 oTBN = mat3(T, B, N);
    gl_Position = perspective * view * model * vec4(aPos, 1.0f);
}