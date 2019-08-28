#include "stdafx.h"

#include "GLVertexBuffer.h"
#include "GLHelperMethods.h"

#include "GLBindVertexBufferCommand.h"

GLVertexBuffer::GLVertexBuffer(unsigned int size, BufferUsage usage, const void* data) :
	VertexBuffer(size),
	GLBufferWrapper(size, usage, BufferType::VERTEX, data)
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
	GetGLCommandList()->BindVertexBuffer(m_id);
	//GetGLCommandList()->Push(std::make_unique<GLBindVertexBufferCommand>(this));
}