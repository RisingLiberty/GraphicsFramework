#include "stdafx.h"

#include "Dx12IndexBuffer.h"
#include "Dx12HelperMethods.h"
#include "Dx12Context.h"


Dx12IndexBuffer::Dx12IndexBuffer(unsigned int count, Format format, Topology topology, BufferUsage usage, BufferAccess access, const void* data) :
	IndexBuffer(count, format, topology),
	Dx12BufferWrapper(m_size, usage, access, data)
{
}

Dx12IndexBuffer::~Dx12IndexBuffer()
{

}

std::unique_ptr<byte> Dx12IndexBuffer::GetData() const
{
	return this->GetDataInternal(m_size);
}

void Dx12IndexBuffer::SetData(const void* data)
{
	this->SetDataInternal(data, m_size);
}

D3D12_INDEX_BUFFER_VIEW Dx12IndexBuffer::GetIndexBufferView() const
{
	D3D12_INDEX_BUFFER_VIEW ib_view;
	ib_view.BufferLocation = this->GetBufferGpu()->GetGPUVirtualAddress();
	ib_view.Format = m_format.ToDirectX();
	ib_view.SizeInBytes = this->GetSize();

	return ib_view;
}
