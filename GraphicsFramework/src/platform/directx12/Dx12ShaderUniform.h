#pragma once

#include "graphics/ShaderUniform.h"

struct Dx12ShaderUniform : public ShaderUniform
{
public:
	Dx12ShaderUniform(const std::string& name, UniformDataType type, unsigned int offset = 0, int size = 0);
	Dx12ShaderUniform(const Dx12ShaderUniform& other);
	virtual ~Dx12ShaderUniform();

public:
	unsigned int offset;
};