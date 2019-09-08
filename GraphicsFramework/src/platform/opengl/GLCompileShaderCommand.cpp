#include "stdafx.h"

#include "GLCompileShaderCommand.h"
#include "GLHelperMethods.h"

namespace
{
	int GetShaderInfoLogLength(unsigned int id)
	{
		int info_length = 0;
		GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_length));
		return info_length;
	}

	std::string GetShaderInfoLog(unsigned int id)
	{
		std::string message;
		int info_length = GetShaderInfoLogLength(id);
		message.resize(info_length);

		GLCALL(glGetShaderInfoLog(id, info_length, nullptr, message.data()));
		return message;
	}
}

GLCompileShaderCommand::GLCompileShaderCommand(unsigned int shaderId):
	m_shader_id(shaderId)
{

}

GLCompileShaderCommand::~GLCompileShaderCommand()
{

}

void GLCompileShaderCommand::Execute()
{
	GLCALL(glCompileShader(m_shader_id));

 	std::string info_log = GetShaderInfoLog(m_shader_id);

	ASSERT(info_log.empty(), "Shader compilation failed");
}