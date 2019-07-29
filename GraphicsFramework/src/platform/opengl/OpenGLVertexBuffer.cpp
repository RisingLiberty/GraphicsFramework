#include "stdafx.h"

#include "OpenGLVertexBuffer.h"
#include "OpenGLHelperMethods.h"

#include "OpenGLDownloadBuffer.h"

namespace
{
	int CustomBufferUsageToGLUsage(BufferUsage usage)
	{
		switch (usage)
		{
		case BufferUsage::DYNAMIC:	return GL_DYNAMIC_DRAW;
		case BufferUsage::STATIC:	return GL_STATIC_DRAW;
		}

		return -1;
	}
}

OpenGLVertexBuffer::OpenGLVertexBuffer(unsigned int size, BufferUsage usage, void* data):
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

std::unique_ptr<DownloadBuffer> OpenGLVertexBuffer::DownloadDataToBuffer() const
{
	std::unique_ptr<DownloadBuffer> buffer = std::make_unique<OpenGLDownloadBuffer>();
	buffer->Download(this);
	return std::move(buffer);
}

void OpenGLVertexBuffer::SetData(const void* data)
{
	this->Bind();
	if (m_usage == BufferUsage::DYNAMIC)
	{
		GLCALL(glBufferData(GL_ARRAY_BUFFER, m_size, data, CustomBufferUsageToGLUsage(m_usage)));
	}
	else
	{
		GLCALL(glBufferStorage(GL_ARRAY_BUFFER, m_size, data, GL_MAP_READ_BIT));
	}
}

void OpenGLVertexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

void OpenGLVertexBuffer::Unbind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

unsigned int OpenGLVertexBuffer::GetId() const
{
	return m_id;
}