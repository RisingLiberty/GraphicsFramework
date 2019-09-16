#include "stdafx.h"

#include "BindShaderProgramCommand.h"

BindShaderProgramCommand::BindShaderProgramCommand(const ShaderProgram* shaderProgram):
	m_shader_program(shaderProgram)
{

}

BindShaderProgramCommand::~BindShaderProgramCommand()
{

}

std::string BindShaderProgramCommand::ToString() const
{
	return "Bind shader program";
}