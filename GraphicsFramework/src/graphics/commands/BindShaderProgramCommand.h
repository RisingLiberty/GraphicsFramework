#pragma once

#include "Command.h"

class ShaderProgram;

class BindShaderProgramCommand : public Command
{
public:
	BindShaderProgramCommand(const ShaderProgram* shaderProgram);
	virtual ~BindShaderProgramCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override = 0;

protected:
	const ShaderProgram* m_shader_program;
};