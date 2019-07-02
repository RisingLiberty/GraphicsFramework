#include "stdafx.h"

#include "Dx11VertexBuffer.h"
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

Dx11VertexBuffer::Dx11VertexBuffer(size_t size, BufferUsage usage, void* data):
	VertexBuffer(size, usage)
{
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = size;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	desc.Usage = CustomBufferUsagToDx11Usage(usage);
	desc.MiscFlags = 0;

	Dx11Context* context = GetDx11Context();

	if (data)
	{
		D3D11_SUBRESOURCE_DATA vertices;
		vertices.pSysMem = data;
		DXCALL(context->GetDevice()->CreateBuffer(&desc, &vertices, m_buffer.ReleaseAndGetAddressOf()));
	}
	else
		DXCALL(context->GetDevice()->CreateBuffer(&desc, NULL, m_buffer.ReleaseAndGetAddressOf()));	
}


Dx11VertexBuffer::~Dx11VertexBuffer()
{
}

void Dx11VertexBuffer::SetData(void* data)
{
	Dx11Context* context = GetDx11Context();
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	DXCALL(context->GetDeviceContext()->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_subresource));
	memcpy(mapped_subresource.pData, data, m_size);
	context->GetDeviceContext()->Unmap(m_buffer.Get(), 0);
}

void Dx11VertexBuffer::Bind() const
{
}

void Dx11VertexBuffer::Unbind() const
{

}

ID3D11Buffer* Dx11VertexBuffer::GetBuffer() const
{
	return m_buffer.Get();
}