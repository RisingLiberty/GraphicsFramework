#include "stdafx.h"

#include "OpenGLIndexBuffer.h"

#include <GL/glew.h>
#include "OpenGLHelperMethods.h"

#include "OpenGLContext.h"

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

OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int  count, Format format, Topology topology, BufferUsage usage, void* data):
	IndexBuffer(count, format, topology, usage)
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
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->GetSize(), data, CustomBufferUsageToGLUsage(m_usage)));
}

unsigned int OpenGLIndexBuffer::GetId() const
{
	return m_id;
}