#pragma once

#include "graphics/CommandList.h"

class Dx12Resource;
class Dx12DescriptorHeap;
class Dx12PipelineState;

class VertexArray;
class IndexBuffer;

class Dx12CommandList : public CommandList
{
public:
	Dx12CommandList();
	virtual ~Dx12CommandList();

    void Open() override;
    void Close() override;

	void ResetAlloc();

	ID3D12GraphicsCommandList* GetApiCommandList() const;

protected:
	ComPtr<ID3D12GraphicsCommandList> m_list;
	ComPtr<ID3D12CommandAllocator> m_cmd_alloc;

	ComPtr<ID3D12PipelineState> m_pipeline_state;
};