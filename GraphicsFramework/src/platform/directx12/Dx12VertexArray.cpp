#include "stdafx.h"

#include "Dx12VertexArray.h"
#include "Dx12HelperMethods.h"
#include "Dx12VertexBuffer.h"

#include "graphics/VertexLayout.h"


namespace
{
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(const Dx12VertexBuffer* vb, const VertexLayout* layout)
	{
		D3D12_VERTEX_BUFFER_VIEW vb_view;
		vb_view.BufferLocation = vb->GetBufferGpu()->GetGPUVirtualAddress();
		vb_view.StrideInBytes = layout->GetSize();
		vb_view.SizeInBytes = (unsigned int)vb->GetSize();

		return vb_view;
	}
}

Dx12VertexArray::Dx12VertexArray(const VertexBuffer* vb, const VertexLayout* layout):
	VertexArray(vb, layout)
{

}

Dx12VertexArray::~Dx12VertexArray()
{

}

void Dx12VertexArray::Bind() const
{
	const Dx12VertexBuffer* dx_vb = dynamic_cast<const Dx12VertexBuffer*>(m_vertex_buffer);
	GetDx12CommandList()->IASetVertexBuffers(0, 1, &GetVertexBufferView(dx_vb, m_vertex_layout));
}

void Dx12VertexArray::Unbind() const
{
}