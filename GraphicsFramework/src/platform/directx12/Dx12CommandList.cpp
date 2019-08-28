#include "stdafx.h"

#include "Dx12HelperMethods.h"
#include "Dx12CommandList.h"

Dx12CommandList::Dx12CommandList()
{
	DXCALL(GetDx12Device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_cmd_alloc.ReleaseAndGetAddressOf())));
	DXCALL(GetDx12Device()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmd_alloc.Get(), nullptr, IID_PPV_ARGS(m_list.ReleaseAndGetAddressOf())));
}

Dx12CommandList::~Dx12CommandList()
{

}

void Dx12CommandList::SetDescriptorHeaps(ID3D12DescriptorHeap* heap)
{
	ID3D12DescriptorHeap* desc_heaps[] = { heap };
	m_list->SetDescriptorHeaps(_countof(desc_heaps), desc_heaps);
}

void Dx12CommandList::SetGraphicsRootSignature(ID3D12RootSignature* rootSignature)
{
	m_list->SetGraphicsRootSignature(rootSignature);
}

void Dx12CommandList::SetGraphicsRootDescriptorTable(D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle)
{
	m_list->SetGraphicsRootDescriptorTable(0, gpu_handle);
}

void Dx12CommandList::SwitchResourceToRenderTarget(ID3D12Resource* resource)
{
	m_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
}

void Dx12CommandList::SwitchResourceToPresent(ID3D12Resource* resource)
{
	m_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}

void Dx12CommandList::SetViewport(D3D12_VIEWPORT viewport)
{
	m_list->RSSetViewports(1, &viewport);
}

void Dx12CommandList::SetScissorRects(D3D12_RECT rect)
{
	m_list->RSSetScissorRects(1, &rect);
}

void Dx12CommandList::SetRenderTargets(D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView, D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView)
{
	m_list->OMSetRenderTargets(1, &renderTargetView, true, &depthStencilView);
}

void Dx12CommandList::SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW ibView)
{
	m_list->IASetIndexBuffer(&ibView);
}

void Dx12CommandList::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
	m_list->IASetPrimitiveTopology(topology);
}

void Dx12CommandList::SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW vbView)
{
	m_list->IASetVertexBuffers(0, 1, &vbView);
}

void Dx12CommandList::SetPipelineState(ComPtr<ID3D12PipelineState> pipelineState)
{
	m_pipeline_state = pipelineState;
	m_list->SetPipelineState(pipelineState.Get());
}

void Dx12CommandList::ResetAlloc()
{
	m_cmd_alloc->Reset();
}

void Dx12CommandList::Reset()
{
	if (!m_pipeline_state.Get())
		spdlog::warn("Resetting command list with empty pipeline state!");

	m_list->Reset(m_cmd_alloc.Get(), m_pipeline_state.Get());
	m_is_closed = false;
}

void Dx12CommandList::Close()
{
	m_list->Close();
	m_is_closed = true;
}

ID3D12GraphicsCommandList* Dx12CommandList::GetApiCommandList() const
{
	return m_list.Get();
}

bool Dx12CommandList::IsClosed() const
{
	return m_is_closed;
}