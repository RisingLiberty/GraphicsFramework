#pragma once

#include "graphics/commands/BindShaderProgramCommand.h"

class GLBindShaderProgramCommand : public BindShaderProgramCommand
{
public:
	GLBindShaderProgramCommand(const ShaderProgram* shaderProgram);
	virtual ~GLBindShaderProgramCommand();

	virtual void Execute() override;
};