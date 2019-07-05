#include "stdafx.h"

#include "Dx12IndexBuffer.h"
#include "Dx12HelperMethods.h"


Dx12IndexBuffer::Dx12IndexBuffer(size_t count, BufferUsage usage, const void* data) :
	IndexBuffer(count, usage)
{
	if (data)
	{
		DXCALL(D3DCreateBlob(sizeof(unsigned int) * count, m_buffer_cpu.ReleaseAndGetAddressOf()));
		memcpy(m_buffer_cpu->GetBufferPointer(), data, sizeof(unsigned int) * count);
		m_buffer_gpu = CreateDefaultBuffer(data, sizeof(unsigned int)*(unsigned int)count, m_upload_buffer);
	}
}

Dx12IndexBuffer::~Dx12IndexBuffer()
{

}

void Dx12IndexBuffer::SetData(const void* data)
{
	assert(!m_buffer_gpu); //don't allow data overwriting

	DXCALL(D3DCreateBlob(sizeof(unsigned int) * (UINT)m_count, m_buffer_cpu.ReleaseAndGetAddressOf()));
	memcpy(m_buffer_cpu->GetBufferPointer(), data, sizeof(unsigned int) * (UINT)m_count);
	m_buffer_gpu = CreateDefaultBuffer(data, sizeof(unsigned int)*(UINT)m_count, m_upload_buffer);
}

void Dx12IndexBuffer::Bind() const
{
	D3D12_INDEX_BUFFER_VIEW ib_view;

	//TODO: make format variable

	ib_view.BufferLocation = m_buffer_gpu->GetGPUVirtualAddress();
	ib_view.Format = DXGI_FORMAT_R32_UINT;
	ib_view.SizeInBytes = (UINT)m_count * sizeof(unsigned int);

	GetDx12CommandList()->IASetIndexBuffer(&ib_view);
}

void Dx12IndexBuffer::Unbind() const
{

}