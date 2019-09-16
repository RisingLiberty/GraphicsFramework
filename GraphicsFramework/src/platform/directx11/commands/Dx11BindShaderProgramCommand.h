#pragma once

#include "graphics/commands/BindShaderProgramCommand.h"

class Dx11BindShaderProgramCommand : public BindShaderProgramCommand
{
public:
	Dx11BindShaderProgramCommand(const ShaderProgram* shaderProgram);
	virtual ~Dx11BindShaderProgramCommand();

	void Execute() override;
};