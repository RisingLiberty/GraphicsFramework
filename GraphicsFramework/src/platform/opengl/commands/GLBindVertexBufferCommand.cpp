#include "stdafx.h"

#include "GLBindVertexBufferCommand.h"
#include "../GLHelperMethods.h"
#include "../GLVertexBuffer.h"

GLBindVertexBufferCommand::GLBindVertexBufferCommand(const VertexBuffer* vb):
	BindVertexBufferCommand(vb)
{

}

GLBindVertexBufferCommand::~GLBindVertexBufferCommand()
{

}

void GLBindVertexBufferCommand::Execute()
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer->As<GLVertexBuffer>()->GetId()));
}
