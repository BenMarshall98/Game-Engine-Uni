#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in ivec4 aIDs;
layout (location = 4) in vec4 aWeights;

uniform mat4 model;

uniform mat4 Bones[100];

void main()
{
	mat4 bone = Bones[aIDs.x] * aWeights.x;
          bone += Bones[aIDs.y] * aWeights.y;
          bone += Bones[aIDs.z] * aWeights.z;
          bone += Bones[aIDs.w] * aWeights.w;

	gl_Position = model * bone * vec4(aPos, 1.0);
}