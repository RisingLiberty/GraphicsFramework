#include "stdafx.h"

#include "GLValidateProgramCommand.h"
#include "../GLHelperMethods.h"

namespace
{
	int GetProgramInfoLength(unsigned int id)
	{
		int message_length = 0;
		GLCALL(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &message_length));
		return message_length;
	}

	std::string GetProgramInfoLog(unsigned int id)
	{
		std::string message;
		int message_length = GetProgramInfoLength(id);

		message.resize(message_length);
		GLCALL(glGetProgramInfoLog(id, message_length, nullptr, message.data()));
		return message;
	}
}

GLValidateProgramCommand::GLValidateProgramCommand(unsigned int programId) :
	m_program_id(programId)
{

}

GLValidateProgramCommand::~GLValidateProgramCommand()
{

}

std::string GLValidateProgramCommand::ToString() const
{
	return "Validate program";
}

void GLValidateProgramCommand::Execute()
{
	GLCALL(glValidateProgram(m_program_id));

	std::string info_log = GetProgramInfoLog(m_program_id);
	if (!info_log.empty())
		spdlog::error(info_log);

}