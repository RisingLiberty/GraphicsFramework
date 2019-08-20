#include "stdafx.h"

#include "Dx12DownloadBuffer.h"
#include "Dx12HelperMethods.h"
#include "Dx12VertexBuffer.h"
#include "Dx12Context.h"
#include "Dx12DirectCommandList.h"

Dx12DownloadBuffer::Dx12DownloadBuffer(unsigned int size):
	DownloadBuffer(size)
{
}

Dx12DownloadBuffer::~Dx12DownloadBuffer()
{
}

void Dx12DownloadBuffer::Download(const ApiBufferWrapper* buffer)
{
	const Dx12VertexBuffer* dx_vb = buffer->As<Dx12VertexBuffer>();

	DXCALL(GetDx12Device()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(m_size),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(m_buffer.GetAddressOf())));

	std::unique_ptr<Dx12CommandList> cmd_list = GetDx12Context()->CreateDirectCommandList();

	cmd_list->GetApiCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		dx_vb->GetBufferGpu(),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_RESOURCE_STATE_COPY_SOURCE));

	cmd_list->GetApiCommandList()->CopyResource(m_buffer.Get(), dx_vb->GetBufferGpu());

	// Uncomment following code in order to execute
	cmd_list->Close();
	cmd_list->Execute();

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
	memcpy(m_data, pReadbackBufferData, m_size);

	D3D12_RANGE emptyRange{ 0, 0 };
	m_buffer->Unmap
	(
		0,
		&emptyRange
	);
}