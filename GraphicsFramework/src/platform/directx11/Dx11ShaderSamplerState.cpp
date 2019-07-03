#include "stdafx.h"

#include "Dx11ShaderSamplerState.h"

Dx11ShaderSamplerState::Dx11ShaderSamplerState(const std::string& name, unsigned int count, unsigned int reg) :
	name(name),
	count(count),
	reg(reg)
{
}

Dx11ShaderSamplerState::~Dx11ShaderSamplerState() = default;