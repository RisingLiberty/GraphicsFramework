#include "stdafx.h"

#include "Dx12UILayer.h"
#include "Dx12HelperMethods.h"
#include "Dx12Context.h"

Dx12UILayer::Dx12UILayer(const std::string& title) :
	UILayer(title)
{
	unsigned int num_frame_resources = GetDx12Context()->GetNrOfFrameResources();
	DXGI_FORMAT back_buffer_format = GetDx12Context()->GetBackBufferFormat();
	ID3D12DescriptorHeap* rtv_descriptor_heap = GetDx12Context()->GetRtvDescriptorHeap();
	ID3D12DescriptorHeap* srv_descriptor_heap = GetDx12Context()->GetSrvDescriptorHeap();

	ImGui_ImplDX12_Init(
		GetDx12Device(), 
		num_frame_resources, 
		back_buffer_format, 
		rtv_descriptor_heap->GetCPUDescriptorHandleForHeapStart(), 
		srv_descriptor_heap->GetGPUDescriptorHandleForHeapStart());
}

Dx12UILayer::~Dx12UILayer()
{
	ImGui_ImplDX12_Shutdown();
}