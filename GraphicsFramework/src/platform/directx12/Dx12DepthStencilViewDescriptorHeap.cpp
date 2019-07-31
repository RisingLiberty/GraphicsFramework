#include "stdafx.h"

#include "Dx12DepthStencilViewDescriptorHeap.h"
#include "Dx12HelperMethods.h"

Dx12DepthStencilViewDescriptorHeap::Dx12DepthStencilViewDescriptorHeap(unsigned int count) :
	Dx12DescriptorHeap(count, DescriptorHeapType::DEPTH_STENCIL)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.NumDescriptors = count;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // can only change when type is cbv, srv or uav
	desc.NodeMask = 0;

	GetDx12Device()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_desc_heap.ReleaseAndGetAddressOf()));
}

Dx12DepthStencilViewDescriptorHeap::~Dx12DepthStencilViewDescriptorHeap()
{

}