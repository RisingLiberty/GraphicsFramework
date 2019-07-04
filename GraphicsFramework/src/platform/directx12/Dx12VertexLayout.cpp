#include "stdafx.h"

#include "Dx12VertexLayout.h"
#include "Dx12HelperMethods.h"
#include "Dx12Context.h"

Dx12VertexLayout::Dx12VertexLayout()
{
	m_layout = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	GetDx12Context()->BindInputLayout(this);
}

Dx12VertexLayout::~Dx12VertexLayout()
{

}

const std::vector<D3D12_INPUT_ELEMENT_DESC>& Dx12VertexLayout::GetLayout() const
{
	return m_layout;
}