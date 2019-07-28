#include "stdafx.h"

#include "Dx11VertexBuffer.h"
#include "Dx11Context.h"
#include "Dx11HelperMethods.h"
#include "Dx11DownloadBuffer.h"

namespace
{
	D3D11_USAGE CustomBufferUsagToDx11Usage(BufferUsage usage)
	{
		switch (usage)
		{
		case BufferUsage::DYNAMIC: return D3D11_USAGE_DYNAMIC;
		case BufferUsage::STATIC: return D3D11_USAGE_IMMUTABLE;
		}

		spdlog::warn("buffer usage not speciifed!");
		return D3D11_USAGE::D3D11_USAGE_DEFAULT;
	}
}

Dx11VertexBuffer::Dx11VertexBuffer(unsigned int size, BufferUsage usage, void* data):
	VertexBuffer(size, usage)
{
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = (unsigned int)size;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = CustomBufferUsagToDx11Usage(usage);
	desc.MiscFlags = 0;

	if (usage == BufferUsage::STATIC)
	{
		ASSERT(data, "data given to immutable buffer is null!");
		desc.CPUAccessFlags = 0; //if not mutable, cpu can't write to buffer.
		D3D11_SUBRESOURCE_DATA vertices;
		vertices.pSysMem = data;
		DXCALL(GetDx11Device()->CreateBuffer(&desc, &vertices, m_buffer.ReleaseAndGetAddressOf()));
	}
	else if (data)
	{
		D3D11_SUBRESOURCE_DATA vertices;
		vertices.pSysMem = data;
		DXCALL(GetDx11Device()->CreateBuffer(&desc, &vertices, m_buffer.ReleaseAndGetAddressOf()));
	}
	else
		DXCALL(GetDx11Device()->CreateBuffer(&desc, NULL, m_buffer.ReleaseAndGetAddressOf()));

	this->DownloadDataToBuffer();
}


Dx11VertexBuffer::~Dx11VertexBuffer()
{
}

std::unique_ptr<DownloadBuffer> Dx11VertexBuffer::DownloadDataToBuffer() const
{
	std::unique_ptr<DownloadBuffer> buffer = std::make_unique<Dx11DownloadBuffer>();
	buffer->Download(this);
	return std::move(buffer);
}

void Dx11VertexBuffer::SetData(const void* data)
{
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	DXCALL(GetDx11DeviceContext()->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_subresource));
	memcpy(mapped_subresource.pData, data, m_size);
	GetDx11DeviceContext()->Unmap(m_buffer.Get(), 0);

	this->DownloadDataToBuffer();
}

ID3D11Buffer* Dx11VertexBuffer::GetBuffer() const
{
	return m_buffer.Get();
}