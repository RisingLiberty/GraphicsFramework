#include "stdafx.h"

#include "Dx12BufferWrapper.h"
#include "Dx12HelperMethods.h"
#include "Dx12DownloadBuffer.h"

Dx12BufferWrapper::Dx12BufferWrapper(unsigned int size, BufferUsage usage, const void* data):
	ApiBufferWrapper(usage)
{
	if (data)
	{
		DXCALL(D3DCreateBlob(size, m_buffer_cpu.ReleaseAndGetAddressOf()));
		memcpy(m_buffer_cpu->GetBufferPointer(), data, size);
		m_buffer_gpu = CreateDefaultBuffer(data, size, m_upload_buffer);
	}
}

Dx12BufferWrapper::~Dx12BufferWrapper()
{
	
}

ID3D12Resource* Dx12BufferWrapper::GetBufferGpu() const
{
	return m_buffer_gpu.Get();
}

void Dx12BufferWrapper::SetDataInternal(const void* data, unsigned int size)
{
	assert(!m_buffer_gpu); // once set, data cannot be set again (for now)
	m_buffer_gpu = CreateDefaultBuffer(data, size, m_upload_buffer);
}

std::unique_ptr<DownloadBuffer> Dx12BufferWrapper::DownloadDataToBuffer(unsigned int size) const
{
	std::unique_ptr<DownloadBuffer> buffer = std::make_unique<Dx12DownloadBuffer>(size);
	buffer->Download(this);
	return std::move(buffer);
}