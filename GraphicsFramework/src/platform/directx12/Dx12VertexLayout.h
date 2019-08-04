#pragma once

#include "graphics/VertexLayout.h"

class Dx12VertexLayout : public VertexLayout
{
public:
	Dx12VertexLayout();
	virtual ~Dx12VertexLayout();

	const std::vector<D3D12_INPUT_ELEMENT_DESC>& GetLayout() const;
	D3D12_INPUT_LAYOUT_DESC GetLayoutDesc() const;

private:
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_layout;
};