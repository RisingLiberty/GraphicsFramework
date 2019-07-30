#include "stdafx.h"

#include "VertexBuffer.h"

#include "platform/opengl/OpenGLVertexBuffer.h"
#include "platform/directx11/Dx11VertexBuffer.h"
#include "platform/directx12/Dx12VertexBuffer.h"
#include "platform/vulkan/VkVertexBuffer.h"

#include "controllers/VertexBufferController.h"

#include "Context.h"

VertexBuffer* VertexBuffer::Create(unsigned int size, BufferUsage usage, void* data)
{
	VertexBufferController* vertex_buffer_controller = Context::GetCurrent()->GetVertexBufferController();
	VertexBuffer* vb = vertex_buffer_controller->Get(size, usage);

	if (vb)
		return vb;

	std::unique_ptr<VertexBuffer> unique_vb;

	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OPENGL:		unique_vb = std::make_unique<OpenGLVertexBuffer>(size, usage, data); break;
	case Context::API::DIRECTX11:	unique_vb = std::make_unique<Dx11VertexBuffer>(size, usage, data); break;
	case Context::API::DIRECTX12:	unique_vb = std::make_unique<Dx12VertexBuffer>(size, usage, data); break;
	case Context::API::VULKAN:		unique_vb = std::make_unique<VkVertexBuffer>(size, usage, data); break;
	}

	vb = unique_vb.get();
	vertex_buffer_controller->Push(unique_vb);
	return vb;
}

VertexBuffer::VertexBuffer(unsigned int size):
	Buffer(size)
{

}

VertexBuffer::~VertexBuffer()
{

}

bool VertexBuffer::operator==(const VertexBuffer* other) const
{
	if (m_resource_id != other->GetResourceId())
		return false;

	if (m_size != other->m_size)
		return false;

	// check for same data/path
	return true;
}