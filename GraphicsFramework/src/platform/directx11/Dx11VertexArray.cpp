#include "stdafx.h"

#include "Dx11VertexArray.h"

#include "Dx11Context.h"

#include "Dx11VertexBuffer.h"
#include "graphics/VertexLayout.h"

namespace
{
	Dx11Context* GetDx11Context()
	{
		return dynamic_cast<Dx11Context*>(Context::GetCurrent());
	}
}

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

	const Dx11VertexBuffer* vs = dynamic_cast<const Dx11VertexBuffer*>(m_vertex_buffer);
	ID3D11Buffer* vertex_buffer = vs->GetBuffer();

	// Max number of input slots: D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT
	GetDx11Context()->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
}

void Dx11VertexArray::Unbind() const
{
	GetDx11Context()->GetDeviceContext()->IASetVertexBuffers(0, 1, nullptr, nullptr, nullptr);
}