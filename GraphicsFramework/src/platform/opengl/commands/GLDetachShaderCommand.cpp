#include "stdafx.h"

#include "GLDetachShaderCommand.h"
#include "../GLHelperMethods.h"

GLDetachShaderCommand::GLDetachShaderCommand(unsigned int programId, unsigned int shaderId) :
	m_program_id(programId),
	m_shader_id(shaderId)
{

}

GLDetachShaderCommand::~GLDetachShaderCommand()
{

}

std::string GLDetachShaderCommand::ToString() const
{
	return "Detach shader";
}

void GLDetachShaderCommand::Execute()
{
	GLCALL(glDetachShader(m_program_id, m_shader_id));
}