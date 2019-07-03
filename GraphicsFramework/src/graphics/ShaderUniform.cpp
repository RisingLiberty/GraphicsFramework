#include "stdafx.h"

#include "ShaderUniform.h"

ShaderUniform::ShaderUniform(int size, UniformDataType type, const std::string& name) :
	size(size),
	type(type),
	name(name)
{
}

ShaderUniform::~ShaderUniform() = default;
