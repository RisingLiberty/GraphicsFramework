#include "stdafx.h"

#include "Dx12DownloadBuffer.h"
#include "Dx12HelperMethods.h"
#include "Dx12VertexBuffer.h"
#include "Dx12Context.h"

Dx12DownloadBuffer::Dx12DownloadBuffer(unsigned int size):
	DownloadBuffer(size)
{

}

Dx12DownloadBuffer::~Dx12DownloadBuffer()
{

}

void Dx12DownloadBuffer::Download(const ApiBufferWrapper* buffer)
{
	const Dx12VertexBuffer* dx_vb = static_cast<const Dx12VertexBuffer*>(buffer);

	DXCALL(GetDx12Device()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(m_size),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(m_buffer.GetAddressOf())));

	GetDx12CommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		dx_vb->GetBufferGpu(),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_RESOURCE_STATE_COPY_SOURCE));

	GetDx12CommandList()->CopyResource(m_buffer.Get(), dx_vb->GetBufferGpu());

	// Uncomment following code in order to execute
	//GetDx12CommandList()->Close();
	//GetDx12Context()->ExecuteCommandQueue();
	//GetDx12Context()->FlushCommandQueue();

	// The code below assumes that the GPU wrote FLOATs to the buffer.

	D3D12_RANGE readbackBufferRange{ 0, m_size };
	FLOAT * pReadbackBufferData{};
	DXCALL(
		m_buffer->Map
		(
			0,
			&readbackBufferRange,
			reinterpret_cast<void**>(&pReadbackBufferData)
		)
	);
		
	// Code goes here to access the data via pReadbackBufferData.

	D3D12_RANGE emptyRange{ 0, 0 };
	m_buffer->Unmap
	(
		0,
		&emptyRange
	);
}