#version 450
#extension GL_ARB_separate_shader_objects : enable

//The inPosition and inColor variables are vertex attributes.
//They're properties that are specified per-vertex in the vertex buffer.
//Just like we manually specified a position and color per vertex using the 2 arrays before.

//Just like fragColor, like the layout(location = x) annotations assign indices to the inputs
//that we can later use to reference them.
//it is important to know that some types, like dvec3 (64 bit vectors) use multiple slots.
//that means that the index after it must be at least 2 higher

layout(binding = 0) uniform UniformBufferObject
{
	mat4 model;
	mat4 view;
	mat4 proj;	
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

out gl_PerVertex
{
	vec4 gl_Position;
};

//vec2 positions[3] = vec2[]
//(
//vec2(0.0, -0.5f),
//vec2(0.5, 0.5),
//vec2(-0.5, 0.5)
//);
//
//vec3 colors[3] = vec3[]
//(
//vec3(1.0, 0.0, 0.0),
//vec3(0.0, 1.0, 0.0),
//vec3(0.0, 0.0, 1.0)
//);

void main() 
{
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0f);
	fragColor = inColor;
	fragTexCoord = inTexCoord;
}