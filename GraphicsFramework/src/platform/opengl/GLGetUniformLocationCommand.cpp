#include "stdafx.h"

#include "GLGetUniformLocationCommand.h"
#include "GLHelperMethods.h"

GLGetUniformLocationCommand::GLGetUniformLocationCommand(unsigned int programId, const std::string& uniformName, int* location):
	m_program_id(programId),
	m_uniform_name(uniformName),
	m_location(location)
{

}

GLGetUniformLocationCommand::~GLGetUniformLocationCommand()
{

}

std::string GLGetUniformLocationCommand::ToString() const
{
	return "Get uniform location";
}

void GLGetUniformLocationCommand::Execute()
{
	GLCALL(*m_location = glGetUniformLocation(m_program_id, m_uniform_name.c_str()));
}