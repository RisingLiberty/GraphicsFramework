#include "stdafx.h"

#include "IndexBuffer.h"

#include "platform/opengl/OpenGLIndexBuffer.h"
#include "platform/directx11/Dx11IndexBuffer.h"
#include "platform/directx12/Dx12IndexBuffer.h"
#include "platform/vulkan/VkIndexBuffer.h"

#include "controllers/IndexBufferController.h"

#include "Context.h"

IndexBuffer* IndexBuffer::Create(unsigned int count, Format format, Topology topology, BufferUsage usage, void* data)
{
	IndexBufferController* index_buffer_controller = Context::GetCurrent()->GetIndexBufferController();
	IndexBuffer* ib = nullptr;// = index_buffer_controller->Get(count, usage);

	if (ib)
		return ib;

	std::unique_ptr<IndexBuffer> unique_ib;

	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OPENGL:		unique_ib = std::make_unique<OpenGLIndexBuffer>(count, format, topology, usage, data); break;
	case Context::API::DIRECTX11:	unique_ib = std::make_unique<Dx11IndexBuffer>(count, format, topology, usage, data); break;
	case Context::API::DIRECTX12:	unique_ib = std::make_unique<Dx12IndexBuffer>(count, format, topology, usage, data); break;
	case Context::API::VULKAN:		unique_ib = std::make_unique<VkIndexBuffer>(count, format, topology, usage, data); break;
	}

	ib = unique_ib.get();
	index_buffer_controller->Push(unique_ib);
	return ib;

}

IndexBuffer::IndexBuffer(unsigned int count, Format format, Topology topology, BufferUsage usage):
	m_count(count),
	m_format(format),
	m_topology(topology),
	m_usage(usage)
{
	if (m_format == EFormat::R8_UINT)
	{
		// calling spdlog::warn 3 times to display message
		// below on another.
		spdlog::warn("Index buffer has unsigned byte format");
		spdlog::warn("This is only available on with OpenGL api");
		spdlog::warn("Using this format can cause undefined behaviour or crashes.");

	}
}

IndexBuffer::~IndexBuffer()
{

}

void IndexBuffer::Bind() const
{
	Context::GetCurrent()->BindIndexBuffer(this);
}

void IndexBuffer::Unbind() const
{
	Context::GetCurrent()->UnbindIndexBuffer(this);
}

unsigned int IndexBuffer::GetCount() const
{
	return m_count;	
}

unsigned int IndexBuffer::GetSize() const
{
	unsigned int size = 0;
	switch (m_format.enum_value)
	{
	case EFormat::R32_UINT:
		size = sizeof(unsigned int);
		break;
	case EFormat::R16_UINT:
		size = sizeof(unsigned short);
		break;
	case EFormat::R8_UINT:
		size = sizeof(unsigned char);
		break;
	default:
		ASSERT(false, "Invalid index buffer format");
	}
	return size * m_count;
}

Format IndexBuffer::GetFormat() const
{
	return m_format;
}

Topology IndexBuffer::GetTopology() const
{
	return m_topology;
}