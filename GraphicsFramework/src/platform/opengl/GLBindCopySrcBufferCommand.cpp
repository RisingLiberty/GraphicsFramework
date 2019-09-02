#include "stdafx.h"

#include "GLBindCopySrcBufferCommand.h"
#include "GLHelperMethods.h"
#include "GLBufferWrapper.h"

GLBindCopySrcBufferCommand::GLBindCopySrcBufferCommand(const ApiBufferWrapper* buffer) :
	BindCopySrcBufferCommand(buffer)
{

}

GLBindCopySrcBufferCommand::~GLBindCopySrcBufferCommand()
{

}

void GLBindCopySrcBufferCommand::Execute()
{
	GLCALL(glBindBuffer(GL_COPY_READ_BUFFER, m_buffer->As<GLBufferWrapper>()->GetId()));
}