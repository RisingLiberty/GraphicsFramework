#include "stdafx.h"

#include "Dx12IndexBuffer.h"
#include "Dx12HelperMethods.h"
#include "Dx12Context.h"


Dx12IndexBuffer::Dx12IndexBuffer(unsigned int  count, Format format, Topology topology, BufferUsage usage, const void* data) :
	IndexBuffer(count, format, topology, usage)
{
	if (data)
		this->SetData(data);
}

Dx12IndexBuffer::~Dx12IndexBuffer()
{

}

void Dx12IndexBuffer::SetData(const void* data)
{
	assert(!m_buffer_gpu); //don't allow data overwriting

	DXCALL(D3DCreateBlob(this->GetSize(), m_buffer_cpu.ReleaseAndGetAddressOf()));
	memcpy(m_buffer_cpu->GetBufferPointer(), data, this->GetSize());
	m_buffer_gpu = CreateDefaultBuffer(data, this->GetSize(), m_upload_buffer);
}

D3D12_INDEX_BUFFER_VIEW Dx12IndexBuffer::GetIndexBufferView() const
{
	D3D12_INDEX_BUFFER_VIEW ib_view;
	ib_view.BufferLocation = m_buffer_gpu->GetGPUVirtualAddress();
	ib_view.Format = m_format.ToDirectX();
	ib_view.SizeInBytes = this->GetSize();

	return ib_view;
}
