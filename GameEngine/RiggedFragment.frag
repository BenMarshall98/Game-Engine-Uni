#version 330 core
in vec2 oTex;

uniform sampler2D texture_map;

out vec4 FragColor;

void main()
{
	FragColor = texture(texture_map, oTex);
}