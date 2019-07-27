#include "stdafx.h"

#include "IndexBufferController.h"

#include "graphics/IndexBuffer.h"

IndexBufferController::IndexBufferController()
{

}

IndexBufferController::~IndexBufferController()
{

}

IndexBuffer* IndexBufferController::Push(std::unique_ptr<IndexBuffer>& ib)
{
	auto it = std::find_if(m_index_buffers.cbegin(), m_index_buffers.cend(),
		[&ib](const std::unique_ptr<IndexBuffer>& uniqueIndexBuffer)
		{
			return uniqueIndexBuffer->GetResourceId() == ib->GetResourceId();
		});

	if (it == m_index_buffers.cend())
	{
		m_index_buffers.push_back(std::move(ib));
		return m_index_buffers.back().get();
	}
	else
	{
		spdlog::warn("layout already exists in controller!");

		// Probably not do this, because user doesn't know 
		// if object is managed without looking at the warning.
		//return layout.release(); 
		ib.reset();
		return it->get();
	}
}