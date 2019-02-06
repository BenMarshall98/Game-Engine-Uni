#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNor;
layout (location = 3) in ivec4 aID;
layout (location = 4) in vec4 aWeight;

const int MAX_BONES = 100;
uniform mat4 BoneMatrix[MAX_BONES];

uniform mat4 model;
uniform mat4 perspective;

out vec2 oTex;

void main()
{
	mat4 BMatrix = BoneMatrix[aID[0]] * aWeight[0];
    BMatrix += BoneMatrix[aID[1]] * aWeight[1];
    BMatrix += BoneMatrix[aID[2]] * aWeight[2];
    BMatrix += BoneMatrix[aID[3]] * aWeight[3];

	oTex = aTex;
    gl_Position = perspective * model * BMatrix * vec4(aPos, 1.0f);
}