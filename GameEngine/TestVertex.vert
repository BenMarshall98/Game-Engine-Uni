#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

out vec2 oTex;
out vec3 oNormal;
out vec3 oPos;

void main()
{
	oTex = aTex;
	oNormal = mat3(transpose(inverse(model))) * aNormal;
	oPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = perspective * view * model * vec4(aPos, 1.0f);
}