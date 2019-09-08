#include "stdafx.h"

#include "GLDeleteShaderCommand.h"
#include "GLHelperMethods.h"

GLDeleteShaderCommand::GLDeleteShaderCommand(unsigned int id):
	m_id(id)
{

}

GLDeleteShaderCommand::~GLDeleteShaderCommand()
{

}

void GLDeleteShaderCommand::Execute()
{
	GLCALL(glDeleteShader(m_id));
}