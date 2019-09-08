#include "stdafx.h"

#include "GLVertexArray.h"
#include "GLHelperMethods.h"
#include "GLVertexBuffer.h"

#include <graphics/VertexLayout.h>

#include <GL/glew.h>

#include "commands/GLBindVertexLayoutCommand.h"

#include "commands/GLCreateVertexArrayCommand.h"
#include "commands/GLDeleteVertexArrayCommand.h"

GLVertexArray::GLVertexArray(const VertexBuffer* vb, const VertexLayout* layout):
	VertexArray(vb, layout)
{
	GetGLContext()->ExecuteDirectCommand(std::make_unique<GLCreateVertexArrayCommand>(&m_id));
	this->EnableAttributes();
}

GLVertexArray::~GLVertexArray()
{
	GetGLContext()->ExecuteDirectCommand(std::make_unique<GLDeleteVertexArrayCommand>(&m_id));
}

unsigned int GLVertexArray::GetId() const
{
	return m_id;
}

void GLVertexArray::EnableAttributes() const
{
	this->Bind();
	m_vertex_buffer->As<GLVertexBuffer>()->GLBind();

	GetGLCommandList()->Push(std::make_unique<GLBindVertexLayoutCommand>(m_vertex_layout));
}
