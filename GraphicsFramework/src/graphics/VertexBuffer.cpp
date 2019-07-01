#include "stdafx.h"

#include "VertexBuffer.h"
#include "platform/opengl/OpenGLVertexBuffer.h"

#include "Context.h"

std::unique_ptr<VertexBuffer> VertexBuffer::Create(size_t size, VertexBuffer::BufferUsage usage)
{
	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OpenGL:
		return std::make_unique<OpenGLVertexBuffer>(size, usage);
	}

	return nullptr;
}

VertexBuffer::VertexBuffer(size_t size, BufferUsage usage):
	m_size(size),
	m_usage(usage)
{

}

VertexBuffer::~VertexBuffer()
{

}