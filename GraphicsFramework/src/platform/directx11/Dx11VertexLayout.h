#pragma once

#include "graphics/VertexLayout.h"

class Dx11VertexLayout : public VertexLayout
{
public:
	Dx11VertexLayout();
	virtual ~Dx11VertexLayout();

	ID3D11InputLayout* GetLayout() const;

private:
	ComPtr<ID3D11InputLayout> m_layout;
};