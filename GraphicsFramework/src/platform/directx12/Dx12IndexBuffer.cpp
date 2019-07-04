#include "stdafx.h"

#include "Dx12IndexBuffer.h"
#include "Dx12HelperMethods.h"


Dx12IndexBuffer::Dx12IndexBuffer(size_t count, BufferUsage usage, void* data) :
	IndexBuffer(count, usage)
{
	if (data)
	{
		ComPtr<ID3D12Resource> intermediate_resource;

		GetDx12Device()->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(count * sizeof(unsigned int)),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(intermediate_resource.GetAddressOf()));

		D3D12_SUBRESOURCE_DATA sub_resource_data = {};
		sub_resource_data.pData = data;
		sub_resource_data.RowPitch = count * sizeof(unsigned int);
		sub_resource_data.SlicePitch = sub_resource_data.RowPitch;

		UpdateSubresources(GetDx12CommandList(), m_buffer.Get(), intermediate_resource.Get(),
			0, 0, 1, &sub_resource_data);
	}
}

Dx12IndexBuffer::~Dx12IndexBuffer()
{

}

void Dx12IndexBuffer::Bind() const
{
	D3D12_INDEX_BUFFER_VIEW ib_view;

	//TODO: make format variable

	ib_view.BufferLocation = m_buffer->GetGPUVirtualAddress();
	ib_view.Format = DXGI_FORMAT_R32_UINT;
	ib_view.SizeInBytes = m_count * sizeof(unsigned int);

	GetDx12CommandList()->IASetIndexBuffer(&ib_view);
}

void Dx12IndexBuffer::Unbind() const
{

}