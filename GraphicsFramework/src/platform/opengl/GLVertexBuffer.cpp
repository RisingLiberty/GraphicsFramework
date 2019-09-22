#include "stdafx.h"

#include "GLVertexBuffer.h"
#include "GLHelperMethods.h"

#include "commands/GLBindVertexBufferCommand.h"

GLVertexBuffer::GLVertexBuffer(unsigned int size, BufferUsage usage, BufferAccess access, const void* data) :
	VertexBuffer(size),
	GLBufferWrapper(size, usage, access, BufferType::VERTEX, data)
{
	if (data)
		this->SetData(data);
}

GLVertexBuffer::~GLVertexBuffer()
{
}

void GLVertexBuffer::SetData(const void* data)
{
	this->GLBind();
	this->SetDataInternal(data, m_size);
}

std::unique_ptr<byte> GLVertexBuffer::GetData() const
{
	return this->GetDataInternal(m_size);
}

void GLVertexBuffer::GLBind() const
{
	GetGLCommandList()->Push<GLBindVertexBufferCommand>(this);
}