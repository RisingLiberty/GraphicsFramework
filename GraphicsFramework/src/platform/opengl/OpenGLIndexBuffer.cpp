#include "stdafx.h"

#include "OpenGLIndexBuffer.h"

#include <GL/glew.h>
#include "OpenGLHelperMethods.h"

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

OpenGLIndexBuffer::OpenGLIndexBuffer(size_t count, BufferUsage usage, void* data):
	IndexBuffer(count, usage)
{
	GLCALL(glGenBuffers(1, &m_id));

	if (data)
		this->SetData(data);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	GLCALL(glDeleteBuffers(1, &m_id));
}

void OpenGLIndexBuffer::SetData(const void* data)
{
	this->Bind();
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), data, CustomBufferUsageToGLUsage(m_usage)));
}

void OpenGLIndexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
}

void OpenGLIndexBuffer::Unbind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}