#pragma once

enum class UniformDataType
{
	UNSPECIFIED,

	INT,
	FLOAT,
	VEC2,
	VEC3,
	VEC4,
	MAT33,
	MAT44,

};

struct ShaderUniform
{
public:
	ShaderUniform(int size, UniformDataType type, const std::string& name);

public:
	unsigned int size;
	UniformDataType type;
	std::string name;
};
