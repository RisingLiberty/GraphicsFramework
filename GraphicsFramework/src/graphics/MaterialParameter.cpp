#include "stdafx.h"

#include "MaterialParameter.h"

MaterialParameter::MaterialParameter()
{
	data = malloc(GetSize());
	ZeroMemory(data, GetSize());
}

MaterialParameter::MaterialParameter(const MaterialParameter& other)
{
	name = other.name;
	data_type = other.data_type;
	data = malloc(GetSize());
	ZeroMemory(data, GetSize());
}

MaterialParameter::MaterialParameter(const MaterialParameter&& other)
{
	name = other.name;
	data_type = other.data_type;
	data = malloc(GetSize());
	ZeroMemory(data, GetSize());
}

MaterialParameter& MaterialParameter::operator=(const MaterialParameter& other)
{
	free(data);

	name = other.name;
	data_type = other.data_type;
	data = malloc(GetSize());
	ZeroMemory(data, GetSize());

	return *this;
}

MaterialParameter& MaterialParameter::operator=(const MaterialParameter&& other)
{
	free(data);

	name = other.name;
	data_type = other.data_type;
	data = malloc(GetSize());
	ZeroMemory(data, GetSize());

	return *this;
}


MaterialParameter::~MaterialParameter()
{
	free(data);
}

unsigned int MaterialParameter::GetSize() const
{
	switch (data_type)
	{
	case MaterialParameterDataType::BOOL:			return sizeof(bool);
	case MaterialParameterDataType::SHORT:			return sizeof(short);
	case MaterialParameterDataType::UNSIGNED_SHORT:	return sizeof(unsigned short);
	case MaterialParameterDataType::INT:			return sizeof(int);
	case MaterialParameterDataType::UNSIGNED_INT:	return sizeof(unsigned int);
	case MaterialParameterDataType::FLOAT:			return sizeof(float);
	case MaterialParameterDataType::DOUBLE:			return sizeof(double);
	case MaterialParameterDataType::VEC2:			return sizeof(float) * 2;
	case MaterialParameterDataType::VEC3:			return sizeof(float) * 3;
	case MaterialParameterDataType::VEC4:			return sizeof(float) * 4;
	case MaterialParameterDataType::MAT3:			return sizeof(float) * 9;
	case MaterialParameterDataType::MAT4:			return sizeof(float) * 16;
	}

	return 0;
}
