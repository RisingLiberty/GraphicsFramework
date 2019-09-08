#include "stdafx.h"

#include "GLBindShaderProgramCommand.h"
#include "GLHelperMethods.h"

GLBindShaderProgramCommand::GLBindShaderProgramCommand(unsigned int programId) :
	m_program_id(programId)
{

}

GLBindShaderProgramCommand::~GLBindShaderProgramCommand()
{

}

void GLBindShaderProgramCommand::Execute()
{
	GLCALL(glUseProgram(m_program_id));
}