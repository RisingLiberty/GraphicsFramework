#pragma once

enum class UniformDataType
{
	UNSPECIFIED,

	INT,
	FLOAT,
	VEC2,
	VEC3,
	VEC4,
	MAT3,
	MAT4,

};

struct ShaderUniform
{
public:
	ShaderUniform(const std::string& name, UniformDataType type, int size = 0);
	virtual ~ShaderUniform();

public:
	std::string name;
	UniformDataType type;
	unsigned int size;
};
