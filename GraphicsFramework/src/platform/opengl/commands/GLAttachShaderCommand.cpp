#include "stdafx.h"

#include "GLAttachShaderCommand.h"
#include "../GLHelperMethods.h"

GLAttachShaderCommand::GLAttachShaderCommand(unsigned int programId, unsigned int shaderId) :
	m_program_id(programId),
	m_shader_id(shaderId)
{

}

GLAttachShaderCommand::~GLAttachShaderCommand()
{

}

std::string GLAttachShaderCommand::ToString() const
{
	return "OpenGL attach shader";
}

void GLAttachShaderCommand::Execute()
{
	GLCALL(glAttachShader(m_program_id, m_shader_id));
}