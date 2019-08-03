#include "stdafx.h"

#include "VertexLayout.h"
#include "Context.h"

#include "platform/directx11/Dx11VertexLayout.h"
#include "platform/directx12/Dx12VertexLayout.h"
#include "platform/vulkan/VkVertexLayout.h"

#include "controllers/VertexLayoutController.h"

VertexLayout::VertexLayout()
{

}

VertexLayout::~VertexLayout()
{

}

VertexLayout* VertexLayout::Create()
{
	VertexLayoutController* vertex_layout_controller = Context::GetCurrent()->GetVertexLayoutController();
	VertexLayout* layout = nullptr;// = vertex_layout_controller->Get();

	if (layout)
		return layout;

	std::unique_ptr<VertexLayout> unique_layout;

	switch (Context::GetApi())
	{
	case API::OPENGL:		unique_layout = std::make_unique<VertexLayout>(); break;
	case API::DIRECTX11:	unique_layout = std::make_unique<Dx11VertexLayout>(); break;
	case API::DIRECTX12:	unique_layout = std::make_unique<Dx12VertexLayout>(); break;
	case API::VULKAN:		unique_layout = std::make_unique<VkVertexLayout>(); break;
	}

	layout = unique_layout.get();
	vertex_layout_controller->Push(unique_layout);
	return layout;
}

void VertexLayout::Clear()
{
	m_size = 0;
	m_attributes.clear();
}