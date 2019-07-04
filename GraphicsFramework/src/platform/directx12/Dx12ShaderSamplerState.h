#pragma once

struct Dx12ShaderSamplerState
{
public:
	Dx12ShaderSamplerState(const std::string& name, unsigned int count, unsigned int reg);
	~Dx12ShaderSamplerState();

public:
	std::string name;
	unsigned int count;
	unsigned int reg;
};