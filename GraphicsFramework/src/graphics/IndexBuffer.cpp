#include "stdafx.h"

#include "IndexBuffer.h"

#include "platform/opengl/OpenGLIndexBuffer.h"
#include "platform/directx11/Dx11IndexBuffer.h"

#include "Context.h"

std::unique_ptr<IndexBuffer> IndexBuffer::Create(size_t count, BufferUsage usage, void* data)
{
	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OpenGL: return std::make_unique<OpenGLIndexBuffer>(count, usage, data);
	case Context::API::DirectX11: return std::make_unique<Dx11IndexBuffer>(count, usage, data);
	}

	return nullptr;
}

IndexBuffer::IndexBuffer(size_t count, BufferUsage usage):
	m_count(count),
	m_usage(usage)
{

}

IndexBuffer::~IndexBuffer()
{

}

size_t IndexBuffer::GetCount() const
{
	return m_count;	
}