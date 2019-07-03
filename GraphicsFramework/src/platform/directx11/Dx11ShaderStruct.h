#pragma once

#include "Dx11ShaderUniform.h"

struct Dx11ShaderStruct : public Dx11ShaderUniform
{
public:
	Dx11ShaderStruct(const std::string& name, const std::vector<Dx11ShaderUniform>& uniforms, unsigned int offset = 0);
	~Dx11ShaderStruct();

public:
	std::vector<Dx11ShaderUniform> uniforms;
};