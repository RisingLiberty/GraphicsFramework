#pragma once

#include "graphics/ShaderUniform.h"
#include "graphics/Shader.h"

struct VkShaderCBuffer
{
public:
	VkShaderCBuffer(const std::string& name, unsigned int boundLocation, std::vector<std::unique_ptr<ShaderUniform>>& uniforms);
	
	~VkShaderCBuffer();

public:
	std::string name;
	unsigned int binding;
	Shader::Type shader_type;
	std::vector<std::unique_ptr<ShaderUniform>> uniforms;
	unsigned int size;
};