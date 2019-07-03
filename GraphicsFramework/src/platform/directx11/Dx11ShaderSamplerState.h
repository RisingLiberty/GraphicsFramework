#pragma once

struct Dx11ShaderSamplerState
{
public:
	Dx11ShaderSamplerState(const std::string& name, unsigned int count, unsigned int reg);
	~Dx11ShaderSamplerState();

public:
	std::string name;
	unsigned int count;
	unsigned int reg;
};