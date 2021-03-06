#include "stdafx.h"

#include "Dx12VertexArray.h"
#include "Dx12HelperMethods.h"
#include "Dx12VertexBuffer.h"
#include "Dx12Context.h"
#include "graphics/VertexLayout.h"

Dx12VertexArray::Dx12VertexArray(const VertexBuffer* vb, const VertexLayout* layout):
	VertexArray(vb, layout)
{
}

Dx12VertexArray::~Dx12VertexArray()
{

}

D3D12_VERTEX_BUFFER_VIEW Dx12VertexArray::GetVertexBufferView() const
{
	const Dx12VertexBuffer* dx_vb = m_vertex_buffer->As<Dx12VertexBuffer>();

	D3D12_VERTEX_BUFFER_VIEW vb_view;
	vb_view.BufferLocation = dx_vb->GetBufferGpu()->GetGPUVirtualAddress();
	vb_view.StrideInBytes = m_vertex_layout->GetSize();
	vb_view.SizeInBytes = (unsigned int)dx_vb->GetSize();

	return vb_view;
}

