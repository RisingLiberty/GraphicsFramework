#pragma once

#include "graphics/ShaderUniform.h"

struct VkShaderStruct : public ShaderUniform
{
public:
	VkShaderStruct(const std::string& name, const std::vector<ShaderUniform>& uniforms, unsigned int offset = 0);
	~VkShaderStruct();

public:
	std::vector<ShaderUniform> uniforms;
};