#include "stdafx.h"

#include "GLBindShaderProgramCommand.h"

#include "../GLHelperMethods.h"
#include "../GLShaderProgram.h"

GLBindShaderProgramCommand::GLBindShaderProgramCommand(const ShaderProgram* shaderProgram) :
	BindShaderProgramCommand(shaderProgram)
{

}

GLBindShaderProgramCommand::~GLBindShaderProgramCommand()
{

}

void GLBindShaderProgramCommand::Execute()
{
	GLCALL(glUseProgram(m_shader_program->As<GLShaderProgram>()->GetId()));
}