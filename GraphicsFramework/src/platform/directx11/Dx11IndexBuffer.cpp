#include "stdafx.h"

#include "Dx11IndexBuffer.h"
#include "Dx11Context.h"
#include "Dx11HelperMethods.h"

namespace
{
	D3D11_USAGE CustomBufferUsagToDx11Usage(BufferUsage usage)
	{
		switch (usage)
		{
		case BufferUsage::DYNAMIC: return D3D11_USAGE_DYNAMIC;
		case BufferUsage::STATIC: return D3D11_USAGE_DYNAMIC;
		}

		spdlog::warn("buffer usage not speciifed!");
		return D3D11_USAGE::D3D11_USAGE_DEFAULT;
	}
}

Dx11IndexBuffer::Dx11IndexBuffer(size_t count, BufferUsage usage, void* data):
	IndexBuffer(count, usage)
{
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = (unsigned int)count * sizeof(unsigned int);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	desc.Usage = CustomBufferUsagToDx11Usage(usage);
	desc.MiscFlags = 0;

	Dx11Context* context = GetDx11Context();

	if (data)
	{
		D3D11_SUBRESOURCE_DATA indices;
		indices.pSysMem = data;
		DXCALL(context->GetDevice()->CreateBuffer(&desc, &indices, m_buffer.ReleaseAndGetAddressOf()));
	}
	else
		DXCALL(context->GetDevice()->CreateBuffer(&desc, NULL, m_buffer.ReleaseAndGetAddressOf()));
}


Dx11IndexBuffer::~Dx11IndexBuffer()
{
}

void Dx11IndexBuffer::SetData(void* data)
{
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	DXCALL(GetDx11Context()->GetDeviceContext()->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_resource));
	memcpy(mapped_resource.pData, data, sizeof(unsigned int) * m_count);
	GetDx11Context()->GetDeviceContext()->Unmap(m_buffer.Get(), 0);
}

void Dx11IndexBuffer::Bind() const
{
	// Index buffer currently always uses uint32_t
	GetDx11Context()->GetDeviceContext()->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Dx11IndexBuffer::Unbind() const
{
	GetDx11Context()->GetDeviceContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
}

ID3D11Buffer* Dx11IndexBuffer::GetBuffer() const
{
	return m_buffer.Get();
}