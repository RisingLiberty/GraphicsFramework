#include "stdafx.h"

#include "GLLinkProgramCommand.h"
#include "../GLHelperMethods.h"

namespace
{
	int GetProgramLinkStatus(unsigned int id)
	{
		int result = 0;
		GLCALL(glGetProgramiv(id, GL_LINK_STATUS, &result));
		return result;
	}
}

GLLinkProgramCommand::GLLinkProgramCommand(unsigned int programId):
	m_program_id(programId)
{

}

GLLinkProgramCommand::~GLLinkProgramCommand()
{

}

std::string GLLinkProgramCommand::ToString() const
{
	return "Link program";
}

void GLLinkProgramCommand::Execute()
{
	GLCALL(glLinkProgram(m_program_id));

	ASSERT(GetProgramLinkStatus(m_program_id) == GL_TRUE, "shader program link error!");
}