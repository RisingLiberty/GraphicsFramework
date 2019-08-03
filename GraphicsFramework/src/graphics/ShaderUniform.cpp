#include "stdafx.h"

#include "ShaderUniform.h"

unsigned int GetUniformDataTypeSize(UniformDataType dataType)
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

	return 0;
}

ShaderUniform::ShaderUniform(const std::string& name, UniformDataType type, unsigned int size, char* data) :
	name(name),
	type(type),
	size(size)
{
	if (size == 0)
		this->size = GetUniformDataTypeSize(type);

	this->data = (char*)malloc(this->size);
	if (data)
		memcpy(this->data, data, this->size);

}

ShaderUniform::ShaderUniform(const ShaderUniform& other):
	name(other.name),
	type(other.type),
	size(other.size)
{
	this->data = (char*)malloc(this->size);
	memcpy(this->data, other.data, this->size);
}

ShaderUniform::~ShaderUniform()
{
	free(this->data);
};
