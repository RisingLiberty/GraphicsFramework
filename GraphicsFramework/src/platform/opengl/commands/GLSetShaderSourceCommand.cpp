#include "stdafx.h"

#include "GLSetShaderSourceCommand.h"
#include "../GLHelperMethods.h"

GLSetShaderSourceCommand::GLSetShaderSourceCommand(unsigned int shaderId, const std::string& code) :
	m_shader_id(shaderId),
	m_code(code)
{

}

GLSetShaderSourceCommand::~GLSetShaderSourceCommand()
{
	
}

std::string GLSetShaderSourceCommand::ToString() const
{
	return "Set shader source";
}

void GLSetShaderSourceCommand::Execute()
{
	const char* code = m_code.c_str();
	GLCALL(glShaderSource(m_shader_id, 1, &code, nullptr));
}