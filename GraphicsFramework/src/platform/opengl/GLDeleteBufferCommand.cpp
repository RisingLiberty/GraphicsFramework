#include "stdafx.h"

#include "GLDeleteBufferCommand.h"
#include "GLHelperMethods.h"

GLDeleteBufferCommand::GLDeleteBufferCommand(unsigned int bufferId):
	m_buffer_id(bufferId)
{

}

GLDeleteBufferCommand::~GLDeleteBufferCommand()
{

}

void GLDeleteBufferCommand::Execute()
{
	GLCALL(glDeleteBuffers(1, &m_buffer_id));
}