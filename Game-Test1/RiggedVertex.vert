#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in ivec4 aIDs;
layout (location = 4) in vec4 aWeights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

out vec4 we;
out vec4 id;

void main()
{
	gl_Position = perspective * view * model * vec4(aPos, 1.0f);

	we = aWeights;
	id = aIDs;
}