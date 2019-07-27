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
		case BufferUsage::STATIC: return D3D11_USAGE_IMMUTABLE;
		}

		spdlog::warn("buffer usage not speciifed!");
		return D3D11_USAGE::D3D11_USAGE_DEFAULT;
	}
}

Dx11IndexBuffer::Dx11IndexBuffer(size_t count, Format format, Topology topology, BufferUsage usage, void* data):
	IndexBuffer(count, format, topology, usage)
{
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = (unsigned int)count * sizeof(unsigned int);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = CustomBufferUsagToDx11Usage(usage);
	desc.MiscFlags = 0;

	if (usage == BufferUsage::STATIC)
	{
		ASSERT(data, "data given to buffer is null!");
		desc.CPUAccessFlags = 0; //if not mutable, cpu can't write to buffer.
		D3D11_SUBRESOURCE_DATA indices;
		indices.pSysMem = data;
		DXCALL(GetDx11Device()->CreateBuffer(&desc, &indices, m_buffer.ReleaseAndGetAddressOf()));
	}
	else if (data)
	{
		D3D11_SUBRESOURCE_DATA indices;
		indices.pSysMem = data;
		DXCALL(GetDx11Device()->CreateBuffer(&desc, &indices, m_buffer.ReleaseAndGetAddressOf()));
	}
	else
		DXCALL(GetDx11Device()->CreateBuffer(&desc, NULL, m_buffer.ReleaseAndGetAddressOf()));
}


Dx11IndexBuffer::~Dx11IndexBuffer()
{
}

void Dx11IndexBuffer::SetData(const void* data)
{
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	DXCALL(GetDx11Context()->GetDeviceContext()->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_resource));
	memcpy(mapped_resource.pData, data, this->GetSize());
	GetDx11DeviceContext()->Unmap(m_buffer.Get(), 0);
}

ID3D11Buffer* Dx11IndexBuffer::GetBuffer() const
{
	return m_buffer.Get();
}