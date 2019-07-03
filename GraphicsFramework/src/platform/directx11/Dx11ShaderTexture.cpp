#include "stdafx.h"

#include "Dx11ShaderTexture.h"

Dx11ShaderTexture::Dx11ShaderTexture(const std::string& name, Type type, unsigned int count, unsigned int reg):
	name(name),
	type(type),
	count(count),
	reg(reg)
{

}

Dx11ShaderTexture::~Dx11ShaderTexture() = default;