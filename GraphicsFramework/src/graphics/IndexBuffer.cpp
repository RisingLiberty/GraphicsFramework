#include "stdafx.h"

#include "IndexBuffer.h"

#include "platform/opengl/OpenGLIndexBuffer.h"
#include "platform/directx11/Dx11IndexBuffer.h"
#include "platform/directx12/Dx12IndexBuffer.h"
#include "platform/vulkan/VkIndexBuffer.h"

#include "controllers/IndexBufferController.h"

#include "Context.h"

IndexBuffer* IndexBuffer::Create(size_t count, Format format, Topology topology, BufferUsage usage, void* data)
{
	IndexBufferController* index_buffer_controller = Context::GetCurrent()->GetIndexBufferController();
	IndexBuffer* ib = nullptr;// = index_buffer_controller->Get(count, usage);

	if (ib)
		return ib;

	std::unique_ptr<IndexBuffer> unique_ib;

	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OPENGL:		unique_ib = std::make_unique<OpenGLIndexBuffer>(count, format, topology, usage, data); break;
	case Context::API::DIRECTX11:	unique_ib = std::make_unique<Dx11IndexBuffer>(count, usage, data); break;
	case Context::API::DIRECTX12:	unique_ib = std::make_unique<Dx12IndexBuffer>(count, usage, data); break;
	case Context::API::VULKAN:		unique_ib = std::make_unique<VkIndexBuffer>(count, usage, data); break;
	}

	ib = unique_ib.get();
	index_buffer_controller->Push(unique_ib);
	return ib;

}

IndexBuffer::IndexBuffer(size_t count, BufferUsage usage, Format format, Topology topology):
	m_count(count),
	m_usage(usage),
	m_format(format),
	m_topology(topology)
{

}

IndexBuffer::~IndexBuffer()
{

}

size_t IndexBuffer::GetCount() const
{
	return m_count;	
}

size_t IndexBuffer::GetSize() const
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