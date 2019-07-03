#pragma once

#include "graphics/ShaderUniform.h"

struct Dx11ShaderUniform : public ShaderUniform
{
public:
	Dx11ShaderUniform(const std::string& name, UniformDataType type, unsigned int offset = 0, int size = 0);
	virtual ~Dx11ShaderUniform();

public:
	unsigned int offset;
};