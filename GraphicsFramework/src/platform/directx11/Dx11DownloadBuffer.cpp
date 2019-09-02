#include "stdafx.h"

#include "Dx11HelperMethods.h"
#include "Dx11DownloadBuffer.h"
#include "Dx11VertexBuffer.h"
#include "Dx11CommandList.h"

Dx11DownloadBuffer::Dx11DownloadBuffer(unsigned int size):
	DownloadBuffer(size)
{
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = 0;
	desc.ByteWidth = m_size;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.MiscFlags = 0;

	GetDx11Device()->CreateBuffer(&desc, NULL, m_buffer.ReleaseAndGetAddressOf());
}

Dx11DownloadBuffer::~Dx11DownloadBuffer()
{

}

void Dx11DownloadBuffer::Download(const ApiBufferWrapper* buffer)
{
	const Dx11VertexBuffer* dx_vb = buffer->As<Dx11VertexBuffer>();
	GetDx11CommandList()->CopyResource(m_buffer.Get(), dx_vb->GetBuffer());

	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	GetDx11CommandList()->Map(m_buffer.Get(), D3D11_MAP_READ, mapped_subresource);
	memcpy(m_cpu_address, mapped_subresource.pData, m_size);
	GetDx11CommandList()->Unmap(m_buffer.Get());
}