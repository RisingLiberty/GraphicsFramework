#include "stdafx.h"

#include "Dx11HelperMethods.h"
#include "Dx11DownloadBuffer.h"
#include "Dx11VertexBuffer.h"
#include "Dx11CommandList.h"

#include "commands/Dx11CopyBufferCommand.h"
#include "commands/Dx11MapBufferCommand.h"
#include "commands/Dx11UnmapBufferCommand.h"

#include "Dx11DirectCommandList.h"

Dx11DownloadBuffer::Dx11DownloadBuffer(unsigned int size):
	DownloadBuffer(size),
	Dx11BufferWrapper(size, EBufferUsage::STATIC, EBufferAccess::READ, BufferType::COPY_DESTINATION, nullptr)
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
	std::unique_ptr<Dx11CommandList> direct_cmd_list = GetDx11Context()->CreateDirectCommandList();
	direct_cmd_list->Push(std::make_unique<Dx11CopyBufferCommand>(m_size, this, buffer));
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	direct_cmd_list->Push(std::make_unique<Dx11MapBufferCommand>(this, D3D11_MAP_READ, &mapped_subresource));
	direct_cmd_list->Close();
	direct_cmd_list->Execute();
	memcpy(m_cpu_address, mapped_subresource.pData, m_size);
	GetDx11CommandList()->Push(std::make_unique<Dx11UnmapBufferCommand>(this));
}