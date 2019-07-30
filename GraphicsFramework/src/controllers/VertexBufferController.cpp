#include "stdafx.h"

#include "VertexBufferController.h"

#include "graphics/VertexBuffer.h"

VertexBufferController::VertexBufferController()
{

}

VertexBufferController::~VertexBufferController()
{

}

VertexBuffer* VertexBufferController::Push(std::unique_ptr<VertexBuffer>& vb)
{
	auto it = std::find_if(m_vertex_buffers.cbegin(), m_vertex_buffers.cend(),
		[&vb](const std::unique_ptr<VertexBuffer>& uniqueVertexBuffer)
		{
			return uniqueVertexBuffer->GetResourceId() == vb->GetResourceId();
		});

	if (it == m_vertex_buffers.cend())
	{
		m_vertex_buffers.push_back(std::move(vb));
		return m_vertex_buffers.back().get();
	}
	else
	{
		spdlog::warn("layout already exists in controller!");

		// Probably not do this, because user doesn't know 
		// if object is managed without looking at the warning.
		//return layout.release(); 
		vb.reset();
		return it->get();

	}

}

VertexBuffer* VertexBufferController::Get(unsigned int resourceId) const
{
	auto it = std::find_if(m_vertex_buffers.cbegin(), m_vertex_buffers.cend(),
		[&resourceId](const std::unique_ptr<VertexBuffer>& vb)
		{
			return vb->GetResourceId() == resourceId;
		});

	if (it != m_vertex_buffers.cend())
		return it->get();
	else
		return nullptr;
}

VertexBuffer* VertexBufferController::Get(VertexBuffer* vb) const
{
	auto it = std::find_if(m_vertex_buffers.cbegin(), m_vertex_buffers.cend(),
		[&vb](const std::unique_ptr<VertexBuffer>& uniqueVb)
		{
			return vb == uniqueVb.get();
		});

	if (it != m_vertex_buffers.cend())
		return it->get();
	else
		return nullptr;

}

VertexBuffer* VertexBufferController::Get(size_t size, BufferUsage usage) const
{
	auto it = std::find_if(m_vertex_buffers.cbegin(), m_vertex_buffers.cend(),
		[&size, &usage](const std::unique_ptr<VertexBuffer>& uniqueVb)
		{
			return uniqueVb->GetSize() == size;
		});

	if (it != m_vertex_buffers.cend())
		return it->get();
	else
		return nullptr;
}