#include "stdafx.h"

#include "GLBindCopyDestBufferCommand.h"
#include "GLHelperMethods.h"
#include "GLBufferWrapper.h"

GLBindCopyDestBufferCommand::GLBindCopyDestBufferCommand(const ApiBufferWrapper* buffer):
	BindCopyDestBufferCommand(buffer)
{

}

GLBindCopyDestBufferCommand::~GLBindCopyDestBufferCommand()
{
	
}

void GLBindCopyDestBufferCommand::Execute()
{
	GLCALL(glBindBuffer(GL_COPY_WRITE_BUFFER, m_buffer->As<GLBufferWrapper>()->GetId()));
}