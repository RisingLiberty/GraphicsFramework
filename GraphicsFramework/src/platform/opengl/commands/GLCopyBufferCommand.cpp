#include "stdafx.h"

#include "GLCopyBufferCommand.h"
#include "../GLHelperMethods.h"

namespace
{
	const unsigned int NULL_OFFSET = 0;
}

GLCopyBufferCommand::GLCopyBufferCommand(unsigned int size, const ApiBufferWrapper* destination, const ApiBufferWrapper* source):
	CopyBufferCommand(size, destination, source)
{

}

GLCopyBufferCommand::~GLCopyBufferCommand()
{

}

void GLCopyBufferCommand::Execute()
{
	GLCALL(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, NULL_OFFSET, NULL_OFFSET, m_size));
}