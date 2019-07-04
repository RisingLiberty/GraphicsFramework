#include "stdafx.h"

#include "Dx12ShaderStruct.h"

namespace
{
	unsigned int CalculateSizeOfBuffer(const std::vector<Dx12ShaderUniform>& uniforms)
	{
		return uniforms.back().offset + uniforms.back().size;
	}
}

Dx12ShaderStruct::Dx12ShaderStruct(const std::string& name, const std::vector<Dx12ShaderUniform>& uniforms, unsigned int offset) :
	Dx12ShaderUniform(name, UniformDataType::UNSPECIFIED, offset, CalculateSizeOfBuffer(uniforms)),
	uniforms(uniforms)
{
}

Dx12ShaderStruct::~Dx12ShaderStruct() = default;