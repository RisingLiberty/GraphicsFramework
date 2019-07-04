#include "stdafx.h"

#include "VertexLayout.h"
#include "Context.h"

#include "platform/directx11/Dx11VertexLayout.h"
#include "platform/directx12/Dx12VertexLayout.h"

VertexLayout::VertexLayout()
{

}

VertexLayout::~VertexLayout()
{

}

std::unique_ptr<VertexLayout> VertexLayout::Create()
{
	switch (Context::GetApi())
	{
	case Context::API::OPENGL: return std::make_unique<VertexLayout>();
	case Context::API::DIRECTX11: return std::make_unique<Dx11VertexLayout>();
	case Context::API::DIRECTX12: return std::make_unique<Dx12VertexLayout>();
		
	}

	return nullptr;
}

void VertexLayout::Clear()
{
	m_size = 0;
	m_attributes.clear();
}