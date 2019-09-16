#pragma once

#include "graphics/CommandList.h"

class Dx12CommandList : public CommandList
{
public:
	Dx12CommandList();
	virtual ~Dx12CommandList();

    void Open() override;
    void Close() override;

	//void SetDescriptorHeaps(ID3D12DescriptorHeap* heap);
	//void SetGraphicsRootSignature(ID3D12RootSignature* rootsignature);
	//void SetGraphicsRootDescriptorTable(D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle);

	//void SwitchResourceToRenderTarget(ID3D12Resource* resource);
	//void SwitchResourceToPresent(ID3D12Resource* resource);
	//void SetViewport(D3D12_VIEWPORT viewport);
	//void SetScissorRects(D3D12_RECT rect);
	//void SetRenderTargets(D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView, D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView);

	//void SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW ibView);
	//void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology);
	//void SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW vbView);

	//void SetPipelineState(ComPtr<ID3D12PipelineState> pipelineState);

	void ResetAlloc();

	ID3D12GraphicsCommandList* GetApiCommandList() const;

protected:
	ComPtr<ID3D12GraphicsCommandList> m_list;
	ComPtr<ID3D12CommandAllocator> m_cmd_alloc;

	ComPtr<ID3D12PipelineState> m_pipeline_state;
};