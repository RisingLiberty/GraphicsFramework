#include "stdafx.h"

#include "Dx11HelperMethods.h"
#include "Dx11DownloadBuffer.h"
#include "Dx11VertexBuffer.h"

Dx11DownloadBuffer::Dx11DownloadBuffer()
{
}

Dx11DownloadBuffer::~Dx11DownloadBuffer()
{

}

void Dx11DownloadBuffer::Download(const VertexBuffer* vb)
{
	if (m_buffer)
		return;

	D3D11_BUFFER_DESC desc;
	desc.BindFlags = 0;
	desc.ByteWidth = (unsigned int)vb->GetSize();
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.MiscFlags = 0;

	GetDx11Device()->CreateBuffer(&desc, NULL, m_buffer.ReleaseAndGetAddressOf());

	const Dx11VertexBuffer* dx_vb = static_cast<const Dx11VertexBuffer*>(vb);
	GetDx11DeviceContext()->CopyResource(m_buffer.Get(), dx_vb->GetBuffer());

	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	DXCALL(GetDx11DeviceContext()->Map(m_buffer.Get(), 0, D3D11_MAP_READ, 0, &mapped_subresource));
	m_data = malloc(vb->GetSize());
	memcpy(m_data, mapped_subresource.pData, vb->GetSize());
	GetDx11DeviceContext()->Unmap(m_buffer.Get(), 0);
}