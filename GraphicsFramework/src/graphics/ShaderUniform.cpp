#include "stdafx.h"

#include "ShaderUniform.h"

size_t GetUniformDataTypeSize(UniformDataType dataType)
{
	switch (dataType)
	{
	case UniformDataType::UNSPECIFIED:	return 0;
	case UniformDataType::INT:			return sizeof(int);
	case UniformDataType::FLOAT:		return sizeof(float);
	case UniformDataType::VEC2:			return sizeof(float) * 2;
	case UniformDataType::VEC3:			return sizeof(float) * 3;
	case UniformDataType::VEC4:			return sizeof(float) * 4;
	case UniformDataType::MAT3:			return sizeof(float) * 9;
	case UniformDataType::MAT4:			return sizeof(float) * 16;
	}
}

ShaderUniform::ShaderUniform(const std::string& name, UniformDataType type, int size) :
	name(name),
	type(type),
	size(size)
{
	if (size == 0)
		this->size = GetUniformDataTypeSize(type);
}

ShaderUniform::~ShaderUniform() = default;
