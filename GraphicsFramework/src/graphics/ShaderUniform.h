#pragma once

enum class UniformDataType
{
	UNSPECIFIED,
	INT,
	UNSIGNED_INT,
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
	ShaderUniform(const std::string& name, UniformDataType type, unsigned int size = 0, char* data = nullptr);
	ShaderUniform(const ShaderUniform& other);
	virtual ~ShaderUniform();

public:
	std::string name;
	UniformDataType type;
	unsigned int size;
	char* data;
};
