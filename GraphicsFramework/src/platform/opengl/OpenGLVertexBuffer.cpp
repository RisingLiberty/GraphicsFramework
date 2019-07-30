#include "stdafx.h"

#include "OpenGLVertexBuffer.h"
#include "OpenGLHelperMethods.h"

#include "OpenGLDownloadBuffer.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(unsigned int size, BufferUsage usage, const void* data) :
	VertexBuffer(size),
	OpenGLBufferWrapper(size, usage, BufferType::VERTEX, data)
{
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
}

void OpenGLVertexBuffer::SetData(const void* data)
{
	this->GLBind();
	this->SetDataInternal(data, m_size);
}

std::unique_ptr<byte> OpenGLVertexBuffer::GetData() const
{
	this->GLBind();
	return this->GetDataInternal(m_size);
}

void OpenGLVertexBuffer::GLBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}