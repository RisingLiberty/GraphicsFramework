#include "stdafx.h"

#include "Dx12UploadBuffer.h"
#include "Dx12HelperMethods.h"

Dx12UploadBuffer::Dx12UploadBuffer(const Dx12ShaderCBuffer& constantBuffer)
{
	unsigned int size = constantBuffer.real_size;

	DXCALL(GetDx12Device()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(constantBuffer.real_size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_upload_buffer.ReleaseAndGetAddressOf())));

	DXCALL(m_upload_buffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mapped_data)));
	m_size = constantBuffer.real_size;
}

Dx12UploadBuffer::~Dx12UploadBuffer()
{
	if (m_upload_buffer)
		m_upload_buffer->Unmap(0, nullptr);
}

void Dx12UploadBuffer::CopyData(const void* data)
{
	memcpy(m_mapped_data, data, m_size);
}

ID3D12Resource* Dx12UploadBuffer::GetResource() const
{
	return m_upload_buffer.Get();
}

unsigned int Dx12UploadBuffer::GetSize() const
{
	return m_size;
}