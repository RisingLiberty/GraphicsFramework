#pragma once

#include "graphics/ShaderUniform.h"

struct Dx12ShaderUniform : public ShaderUniform
{
public:
	Dx12ShaderUniform(const std::string& name, UniformDataType type, unsigned int offset = 0, int size = 0);
	virtual ~Dx12ShaderUniform();

public:
	unsigned int offset;
};