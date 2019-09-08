#include "stdafx.h"

#include "GLBindIndexBufferCommand.h"
#include "../GLHelperMethods.h"
#include "../GLIndexBuffer.h"

GLBindIndexBufferCommand::GLBindIndexBufferCommand(const IndexBuffer* ib):
	BindIndexBufferCommand(ib)
{

}

GLBindIndexBufferCommand::~GLBindIndexBufferCommand()
{

}

void GLBindIndexBufferCommand::Execute()
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer->As<GLIndexBuffer>()->GetId()));
}