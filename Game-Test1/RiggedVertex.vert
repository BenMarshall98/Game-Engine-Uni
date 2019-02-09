#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in ivec4 aIDs;
layout (location = 4) in vec4 aWeights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

uniform mat4 Bones[100];

out vec4 we;
out vec4 id;

void main()
{
	mat4 bone = Bones[aIDs[0]] * aWeights[0];
	bone += Bones[aIDs[1]] * aWeights[1];
	bone += Bones[aIDs[2]] * aWeights[2];
	bone += Bones[aIDs[3]] * aWeights[3];

	gl_Position = perspective * view * model * bone * vec4(aPos, 1.0f);

	we = aWeights;
	id = aIDs;
}