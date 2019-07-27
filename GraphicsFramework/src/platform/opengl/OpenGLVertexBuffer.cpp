#include "stdafx.h"

#include "OpenGLVertexBuffer.h"
#include "OpenGLHelperMethods.h"

#include <GL/glew.h>

namespace
{
	int CustomBufferUsageToGLUsage(BufferUsage usage)
	{
		switch (usage)
		{
		case BufferUsage::DYNAMIC:
			return GL_STATIC_DRAW;
		case BufferUsage::STATIC:
			return GL_DYNAMIC_DRAW;
		}

		return 0;
	}
}

OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size, BufferUsage usage, void* data):
	VertexBuffer(size, usage)
{
	GLCALL(glGenBuffers(1, &m_id));

	if (data != nullptr)
		this->SetData(data);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	GLCALL(glDeleteBuffers(1, &m_id));
}

void OpenGLVertexBuffer::SetData(const void* data)
{
	this->ForceBind();
	GLCALL(glBufferData(GL_ARRAY_BUFFER, m_size, data, CustomBufferUsageToGLUsage(m_usage)));
}

void OpenGLVertexBuffer::ForceBind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

void OpenGLVertexBuffer::ForceUnbind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

unsigned int OpenGLVertexBuffer::GetId() const
{
	return m_id;
}