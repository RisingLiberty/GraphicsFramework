#include "stdafx.h"

#include "GLIndexBuffer.h"

#include <GL/glew.h>
#include "GLHelperMethods.h"

#include "GLContext.h"

GLIndexBuffer::GLIndexBuffer(unsigned int count, Format format, Topology topology, BufferUsage usage, BufferAccess access, void* data):
	IndexBuffer(count, format, topology),
	GLBufferWrapper(m_size, usage, access, BufferType::INDEX, data)
{
	if (data)
		this->SetData(data);
}

GLIndexBuffer::~GLIndexBuffer()
{

}

void GLIndexBuffer::SetData(const void* data)
{
	this->Bind();
	this->SetDataInternal(data, m_size);
}

std::unique_ptr<byte> GLIndexBuffer::GetData() const
{
	return this->GetDataInternal(m_size);
}

void GLIndexBuffer::GLBind() const
{
	this->Bind();
}
