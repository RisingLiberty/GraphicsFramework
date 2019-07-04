#pragma once

#include "Dx12ShaderUniform.h"

struct Dx12ShaderStruct : public Dx12ShaderUniform
{
public:
	Dx12ShaderStruct(const std::string& name, const std::vector<Dx12ShaderUniform>& uniforms, unsigned int offset = 0);
	~Dx12ShaderStruct();

public:
	std::vector<Dx12ShaderUniform> uniforms;
};