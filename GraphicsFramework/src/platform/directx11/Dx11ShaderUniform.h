#pragma once

#include "graphics/ShaderUniform.h"

struct Dx11ShaderUniform : public ShaderUniform
{
public:
	Dx11ShaderUniform(int size, UniformDataType type, const std::string& name);
	virtual ~Dx11ShaderUniform();

};