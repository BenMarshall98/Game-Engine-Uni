#version 330

in vec4 we;
in vec4 id;

out vec4 FragColor;

void main()
{
	FragColor = vec4(we.xyz, 1.0);
}