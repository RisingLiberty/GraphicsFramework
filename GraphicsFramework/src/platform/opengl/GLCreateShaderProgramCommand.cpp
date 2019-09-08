#include "stdafx.h"

#include "GLCreateShaderProgramCommand.h"
#include "GLHelperMethods.h"

GLCreateShaderProgramCommand::GLCreateShaderProgramCommand(unsigned int* id):
	m_id(id)
{

}

GLCreateShaderProgramCommand::~GLCreateShaderProgramCommand()
{

}

std::string GLCreateShaderProgramCommand::ToString() const
{
	return "Create shader program";
}

void GLCreateShaderProgramCommand::Execute()
{
	GLCALL(*m_id = glCreateProgram());
}