#include "stdafx.h"

#include "OpenGLVertexArray.h"
#include "OpenGLHelperMethods.h"
#include "OpenGLVertexBuffer.h"

#include <graphics/VertexLayout.h>

#include <GL/glew.h>

OpenGLVertexArray::OpenGLVertexArray(const VertexBuffer* vb, const VertexLayout* layout):
	VertexArray(vb, layout)
{
	GLCALL(glGenVertexArrays(1, &m_id));

	this->Bind();
	vb->Bind();

	const std::vector<VertexAttribute>& attributes = layout->GetAttributes();
	size_t offset = 0;

	for (unsigned int i = 0; i < attributes.size(); ++i)
	{
		const VertexAttribute& attribute = attributes[i];
		GLCALL(glEnableVertexAttribArray(i));
		GLCALL(glVertexAttribPointer(i, attribute.count, attribute.GetOpenGLDataType(), attribute.is_normalized, layout->GetSize(), reinterpret_cast<const void*>(offset)));
		offset += attribute.GetSize();
	}
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	GLCALL(glDeleteVertexArrays(1, &m_id));
}

void OpenGLVertexArray::Bind() const
{
	GLCALL(glBindVertexArray(m_id));
}

void OpenGLVertexArray::Unbind() const
{
	GLCALL(glBindVertexArray(0));
}
