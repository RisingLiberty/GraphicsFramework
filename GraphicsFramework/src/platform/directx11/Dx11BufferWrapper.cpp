#include "stdafx.h"

#include "Dx11HelperMethods.h"
#include "Dx11BufferWrapper.h"
#include "Dx11DownloadBuffer.h"

#include "Dx11CommandList.h"

#include "commands/Dx11MapBufferCommand.h"
#include "commands/Dx11UnmapBufferCommand.h"

Dx11BufferWrapper::Dx11BufferWrapper(unsigned int size, BufferUsage usage, BufferAccess access, BufferType type, const void* data):
	ApiBufferWrapper(usage, access)
{
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = type == BufferType::VERTEX ? D3D11_BIND_VERTEX_BUFFER : D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = (unsigned int)size;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = m_usage.ToDirectX11();
	desc.MiscFlags = 0;

	if (m_usage == EBufferUsage::STATIC)
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

}

Dx11BufferWrapper::~Dx11BufferWrapper()
{

}

ID3D11Buffer* Dx11BufferWrapper::GetBuffer() const
{
	return m_buffer.Get();
}

void Dx11BufferWrapper::SetDataInternal(const void* data, unsigned int size)
{
	this->SetDataInternal(data, size);
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;

	GetDx11CommandList()->Push(std::make_unique<Dx11MapBufferCommand>(this, D3D11_MAP_WRITE_NO_OVERWRITE, &mapped_subresource));
	memcpy(mapped_subresource.pData, data, size);
	GetDx11CommandList()->Push(std::make_unique<Dx11UnmapBufferCommand>(this));
}

std::unique_ptr<DownloadBuffer> Dx11BufferWrapper::DownloadDataToBuffer(unsigned int size) const
{
	std::unique_ptr<DownloadBuffer> buffer = std::make_unique<Dx11DownloadBuffer>(size);
	buffer->Download(this);
	return std::move(buffer);
}