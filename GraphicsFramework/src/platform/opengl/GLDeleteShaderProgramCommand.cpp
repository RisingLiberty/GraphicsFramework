#include "stdafx.h"

#include "GLDeleteShaderProgramCommand.h"
#include "GLHelperMethods.h"

GLDeleteShaderProgramCommand::GLDeleteShaderProgramCommand(unsigned int id):
	m_id(id)
{

}

GLDeleteShaderProgramCommand::~GLDeleteShaderProgramCommand()
{

}

std::string GLDeleteShaderProgramCommand::ToString() const
{
	return "Delete shader program";
}

void GLDeleteShaderProgramCommand::Execute()
{
	GLCALL(glDeleteProgram(m_id));
}