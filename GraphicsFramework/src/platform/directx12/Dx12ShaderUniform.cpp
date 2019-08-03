#include "stdafx.h"

#include "Dx12ShaderUniform.h"

Dx12ShaderUniform::Dx12ShaderUniform(const std::string& name, UniformDataType type, unsigned int offset, int size):
	ShaderUniform(name, type, size),
	offset(offset)
{

}

Dx12ShaderUniform::Dx12ShaderUniform(const Dx12ShaderUniform& other):
	ShaderUniform(other),
	offset(other.offset)
{

}

Dx12ShaderUniform::~Dx12ShaderUniform()
{

}

