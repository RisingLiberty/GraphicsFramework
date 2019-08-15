#include "stdafx.h"

#include "OpenGLIndexBuffer.h"

#include <GL/glew.h>
#include "OpenGLHelperMethods.h"

#include "OpenGLContext.h"

OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int count, Format format, Topology topology, BufferUsage usage, void* data):
	IndexBuffer(count, format, topology),
	OpenGLBufferWrapper(m_size, usage, BufferType::INDEX, data)
{
	m_id = GetOpenGLCommandList()->CreateBuffer();

	if (data)
		this->SetData(data);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	GetOpenGLCommandList()->DeleteBuffer(m_id);
}

std::unique_ptr<byte> OpenGLIndexBuffer::GetData() const
{
	return this->GetDataInternal(m_size);
}

void OpenGLIndexBuffer::SetData(const void* data)
{
	this->Bind();
	this->SetDataInternal(data, m_size);
}

void OpenGLIndexBuffer::GLBind() const
{
	this->Bind();
}

unsigned int OpenGLIndexBuffer::GetId() const
{
	return m_id;
}