#pragma once

#include "graphics/DescriptorHeap.h"

class Dx12DescriptorHeap : public DescriptorHeap
{
public:
	Dx12DescriptorHeap(unsigned int count, DescriptorHeapType type);
	virtual ~Dx12DescriptorHeap();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuView() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuView() const;

	ID3D12DescriptorHeap* GetHeap() const;

protected:
	ComPtr<ID3D12DescriptorHeap> m_desc_heap;
};