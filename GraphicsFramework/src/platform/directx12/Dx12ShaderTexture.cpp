#include "stdafx.h"

#include "Dx12ShaderTexture.h"

Dx12ShaderTexture::Dx12ShaderTexture(const std::string& name, Type type, unsigned int count, unsigned int reg):
	name(name),
	type(type),
	count(count),
	reg(reg)
{

}

Dx12ShaderTexture::~Dx12ShaderTexture() = default;