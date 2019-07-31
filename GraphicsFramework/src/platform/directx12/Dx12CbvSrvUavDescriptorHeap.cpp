#include "stdafx.h"

#include "Dx12CbvSrvUavDescriptorHeap.h"
#include "Dx12HelperMethods.h"

Dx12CbvSrvUavDescriptorHeap::Dx12CbvSrvUavDescriptorHeap(unsigned int count, bool isVisibleByShader):
	Dx12DescriptorHeap(count, DescriptorHeapType::CBV_SRV_UAV)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.NumDescriptors = count;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.Flags = isVisibleByShader ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // can only change when type is cbv, srv or uav
	desc.NodeMask = 0;

	GetDx12Device()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_desc_heap.ReleaseAndGetAddressOf()));
}

Dx12CbvSrvUavDescriptorHeap::~Dx12CbvSrvUavDescriptorHeap()
{

}