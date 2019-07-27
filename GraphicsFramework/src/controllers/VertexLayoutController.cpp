#include "stdafx.h"

#include "VertexLayoutController.h"

#include "graphics/VertexLayout.h"

VertexLayoutController::VertexLayoutController()
{

}

VertexLayoutController::~VertexLayoutController()
{

}

VertexLayout* VertexLayoutController::Push(std::unique_ptr<VertexLayout>& layout)
{
	auto it = std::find_if(m_layouts.cbegin(), m_layouts.cend(),
		[&layout](const std::unique_ptr<VertexLayout>& uniqueLayout)
		{
			return uniqueLayout->GetResourceId() == layout->GetResourceId();
		});

	if (it == m_layouts.cend())
	{
		m_layouts.push_back(std::move(layout));
		return m_layouts.back().get();
	}
	else
	{
		spdlog::warn("layout already exists in controller!");

		// Probably not do this, because user doesn't know 
		// if object is managed without looking at the warning.
		//return layout.release(); 
		layout.reset();
		return it->get();

	}
}