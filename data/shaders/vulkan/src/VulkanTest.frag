#version 450
#extension GL_ARB_separate_shader_objects : enable

//There are equivalent sampler1D and sampler3D types for other types or images.
//Make sure to set the correct binding here.
layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main()
{
	//Textures are sampled using the built-in texture function
	//It takes a sampler and coordinate as arguments.
	//The sampler automatically takes care of the filtering and transformations
	//in the background.

	//Debugging
	//outColor = vec4(fragTexCoord, 0.0f, 1.0f);

	//Regular texture
	outColor = texture(texSampler, fragTexCoord);

	//texture + color
	//outColor = vec4(fragColor * texture(texSampler, fragTexCoord).rgb, 1.0f);

	//Texturing beyond dimensions
	//outColor = texture(texSampler, fragTexCoord * 2.0f);
}