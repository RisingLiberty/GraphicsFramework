#include "stdafx.h"

#include "GLCreateShaderCommand.h"
#include "GLHelperMethods.h"

GLCreateShaderCommand::GLCreateShaderCommand(ShaderType type, unsigned int* id):
	CreateShaderCommand(type),
	m_id(id)
{

}

GLCreateShaderCommand::~GLCreateShaderCommand()
{

}

void GLCreateShaderCommand::Execute()
{
	switch (m_type)
	{
	case ShaderType::VERTEX:	GLCALL(*m_id = glCreateShader(GL_VERTEX_SHADER));	break;
	case ShaderType::FRAGMENT:	GLCALL(*m_id = glCreateShader(GL_FRAGMENT_SHADER)); break;
	}
}