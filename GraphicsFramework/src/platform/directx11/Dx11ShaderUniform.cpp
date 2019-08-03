#include "stdafx.h"

#include "Dx11ShaderUniform.h"

Dx11ShaderUniform::Dx11ShaderUniform(const std::string& name, UniformDataType type, unsigned int offset, int size):
	ShaderUniform(name, type, size),
	offset(offset)
{

}

Dx11ShaderUniform::Dx11ShaderUniform(const Dx11ShaderUniform& other):
	ShaderUniform(other),
	offset(other.offset)
{

}

Dx11ShaderUniform::~Dx11ShaderUniform()
{

}

