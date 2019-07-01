#version 330 core

in vec2 position;
in vec3 color;
in vec2 texcoord;

// The color to output to the fragment shader
out vec3 Color;
out vec2 Texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(position, 0.0f, 1.0f);
	Color = color;
	Texcoord = texcoord;
};