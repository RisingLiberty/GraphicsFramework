#include "stdafx.h"

#include "GLVertexArray.h"
#include "GLHelperMethods.h"
#include "GLVertexBuffer.h"

#include <graphics/VertexLayout.h>

#include <GL/glew.h>

GLVertexArray::GLVertexArray(const VertexBuffer* vb, const VertexLayout* layout):
	VertexArray(vb, layout)
{
	m_id = GetGLCommandList()->CreateVertexArray();

	this->EnableAttributes();
}

GLVertexArray::~GLVertexArray()
{
	GetGLCommandList()->DeleteVertexArray(m_id);
}

unsigned int GLVertexArray::GetId() const
{
	return m_id;
}

void GLVertexArray::EnableAttributes() const
{
	this->Bind();
	m_vertex_buffer->As<GLVertexBuffer>()->GLBind();

	const std::vector<VertexAttribute>& attributes = m_vertex_layout->GetAttributes();
	size_t offset = 0;

	for (unsigned int i = 0; i < attributes.size(); ++i)
	{
		const VertexAttribute& attribute = attributes[i];
		GLCALL(glEnableVertexAttribArray(i));
		GLCALL(glVertexAttribPointer(i, attribute.count, attribute.GetGLDataType(), attribute.is_normalized, m_vertex_layout->GetSize(), reinterpret_cast<const void*>(offset)));
		offset += attribute.GetSize();
	}
}
