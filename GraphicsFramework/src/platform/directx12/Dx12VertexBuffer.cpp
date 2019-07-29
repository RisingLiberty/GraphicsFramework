#include "stdafx.h"

#include "Dx12VertexBuffer.h"
#include "Dx12HelperMethods.h"
#include "Dx12DownloadBuffer.h"

Dx12VertexBuffer::Dx12VertexBuffer(unsigned int size, BufferUsage usage, const void* data):
	VertexBuffer(size, usage)
{
	if (data)
	{
		DXCALL(D3DCreateBlob(size, m_buffer_cpu.ReleaseAndGetAddressOf()));
		memcpy(m_buffer_cpu->GetBufferPointer(), data, size);
		m_buffer_gpu = CreateDefaultBuffer(data, (unsigned int)size, m_upload_buffer);
	}

	this->DownloadDataToBuffer();
}

Dx12VertexBuffer::~Dx12VertexBuffer()
{

}

std::unique_ptr<DownloadBuffer> Dx12VertexBuffer::DownloadDataToBuffer() const
{
	std::unique_ptr<DownloadBuffer> buffer = std::make_unique<Dx12DownloadBuffer>();
	buffer->Download(this);
	return std::move(buffer);
}

void Dx12VertexBuffer::SetData(const void* data)
{
	assert(!m_buffer_gpu); // once set, data cannot be set again (for now)
	m_buffer_gpu = CreateDefaultBuffer(data, (unsigned int)m_size, m_upload_buffer);
}

ID3D12Resource* Dx12VertexBuffer::GetBufferGpu() const
{
	return m_buffer_gpu.Get();
}