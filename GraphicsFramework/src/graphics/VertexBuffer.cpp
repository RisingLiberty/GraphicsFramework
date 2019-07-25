#include "stdafx.h"

#include "VertexBuffer.h"

#include "platform/opengl/OpenGLVertexBuffer.h"
#include "platform/directx11/Dx11VertexBuffer.h"
#include "platform/directx12/Dx12VertexBuffer.h"
#include "platform/vulkan/VkVertexBuffer.h"

#include "Context.h"

std::unique_ptr<VertexBuffer> VertexBuffer::Create(size_t size, BufferUsage usage, void* data)
{
	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OPENGL: return std::make_unique<OpenGLVertexBuffer>(size, usage, data);
	case Context::API::DIRECTX11: return std::make_unique<Dx11VertexBuffer>(size, usage, data);
	case Context::API::DIRECTX12: return std::make_unique<Dx12VertexBuffer>(size, usage, data);
	case Context::API::VULKAN: return std::make_unique<VkVertexBuffer>(size, usage, data);
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

size_t VertexBuffer::GetSize() const
{
	return m_size;
}