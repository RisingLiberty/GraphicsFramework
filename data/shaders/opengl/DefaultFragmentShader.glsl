#version 330 core

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

void main()
{
	vec3 finalColor = Color;

	outColor = vec4(finalColor, 1.0f);
};