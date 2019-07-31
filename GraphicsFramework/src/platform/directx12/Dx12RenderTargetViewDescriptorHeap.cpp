#include "stdafx.h"

#include "Dx12HelperMethods.h"
#include "Dx12RenderTargetViewDescriptorHeap.h"

Dx12RenderTargetViewDescriptorHeap::Dx12RenderTargetViewDescriptorHeap(unsigned int count):
	Dx12DescriptorHeap(count, DescriptorHeapType::RENDER_TARGET)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.NumDescriptors = count;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // can only change when type is cbv, srv or uav
	desc.NodeMask = 0;

	GetDx12Device()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_desc_heap.ReleaseAndGetAddressOf()));
}

Dx12RenderTargetViewDescriptorHeap::~Dx12RenderTargetViewDescriptorHeap()
{

}