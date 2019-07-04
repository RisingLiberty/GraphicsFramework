#include "stdafx.h"

#include "Dx11VertexArray.h"

#include "Dx11Context.h"
#include "Dx11HelperMethods.h"
#include "Dx11VertexBuffer.h"
#include "Dx11VertexLayout.h"
#include "Dx11VertexShader.h"

Dx11VertexArray::Dx11VertexArray(const VertexBuffer* vb, const VertexLayout* layout):
	VertexArray(vb, layout)
{
}


Dx11VertexArray::~Dx11VertexArray()
{
}

void Dx11VertexArray::Bind() const
{
	unsigned int offset = 0;
	unsigned int stride = m_vertex_layout->GetSize();

	const Dx11VertexBuffer* vb = dynamic_cast<const Dx11VertexBuffer*>(m_vertex_buffer);
	ID3D11Buffer* vertex_buffer = vb->GetBuffer();

	// Max number of input slots: D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT
	GetDx11DeviceContext()->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
	GetDx11DeviceContext()->IASetInputLayout(dynamic_cast<const Dx11VertexLayout*>(m_vertex_layout)->GetLayout());
}

void Dx11VertexArray::Unbind() const
{
	GetDx11DeviceContext()->IASetVertexBuffers(0, 1, nullptr, nullptr, nullptr);
}