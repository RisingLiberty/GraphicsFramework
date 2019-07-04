#include "stdafx.h"

#include "Dx12ShaderSamplerState.h"

Dx12ShaderSamplerState::Dx12ShaderSamplerState(const std::string& name, unsigned int count, unsigned int reg) :
	name(name),
	count(count),
	reg(reg)
{
}

Dx12ShaderSamplerState::~Dx12ShaderSamplerState() = default;