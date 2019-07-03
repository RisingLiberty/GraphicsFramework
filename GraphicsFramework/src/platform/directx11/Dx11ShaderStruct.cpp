#include "stdafx.h"

#include "Dx11ShaderStruct.h"

namespace
{
	unsigned int CalculateSizeOfBuffer(const std::vector<Dx11ShaderUniform>& uniforms)
	{
		return uniforms.back().offset + uniforms.back().size;
	}
}

Dx11ShaderStruct::Dx11ShaderStruct(const std::string& name, const std::vector<Dx11ShaderUniform>& uniforms, unsigned int offset) :
	Dx11ShaderUniform(name, UniformDataType::UNSPECIFIED, offset, CalculateSizeOfBuffer(uniforms)),
	uniforms(uniforms)
{
}

Dx11ShaderStruct::~Dx11ShaderStruct() = default;