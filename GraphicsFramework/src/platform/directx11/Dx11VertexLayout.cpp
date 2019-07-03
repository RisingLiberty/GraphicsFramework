#include "stdafx.h"

#include "Dx11VertexLayout.h"
#include "Dx11VertexShader.h"
#include "Dx11HelperMethods.h"
#include "Dx11Context.h"

Dx11VertexLayout::Dx11VertexLayout()
{
	D3D11_INPUT_ELEMENT_DESC desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	Dx11VertexShader* vs = GetDx11Context()->GetBoundVertexShader();

	GetDx11Device()->CreateInputLayout(desc, 1, vs->GetCompiledCode()->GetBufferPointer(), vs->GetCompiledCode()->GetBufferSize(), m_layout.ReleaseAndGetAddressOf());
}

Dx11VertexLayout::~Dx11VertexLayout()
{

}

ID3D11InputLayout* Dx11VertexLayout::GetLayout() const
{
	return m_layout.Get();
}