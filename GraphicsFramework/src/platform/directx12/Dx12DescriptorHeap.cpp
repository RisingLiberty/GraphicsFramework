#include "stdafx.h"

#include "Dx12DescriptorHeap.h"
#include "Dx12HelperMethods.h"

Dx12DescriptorHeap::Dx12DescriptorHeap(unsigned int count, DescriptorHeapType type) :
	DescriptorHeap(count, type)
{
}

Dx12DescriptorHeap::~Dx12DescriptorHeap()
{
	
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12DescriptorHeap::GetCpuView() const
{
	return m_desc_heap->GetCPUDescriptorHandleForHeapStart();
}

D3D12_GPU_DESCRIPTOR_HANDLE Dx12DescriptorHeap::GetGpuView() const
{
	return m_desc_heap->GetGPUDescriptorHandleForHeapStart();
}

ID3D12DescriptorHeap* Dx12DescriptorHeap::GetHeap() const
{
	return m_desc_heap.Get();
}