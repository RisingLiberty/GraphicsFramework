#include "stdafx.h"

#include "Dx12Resource.h"
#include "Dx12HelperMethods.h"

#include "Dx12CommandList.h"

#include "commands/Dx12SwitchResourceStateCommand.h"

Dx12Resource::Dx12Resource(ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES resourceState) :
    m_resource(resource),
    m_current_state(resourceState)
{

}

Dx12Resource::Dx12Resource(unsigned int size, const void* data, ID3D12Resource* uploadBuffer):
    m_current_state(D3D12_RESOURCE_STATE_COMMON)
{
    ID3D12Device* device = GetDx12Device();
    Dx12CommandList* cmd_list = GetDx12CommandList();
    
    // Create the actual default buffer resource
    DXCALL(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(size),
        D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_PPV_ARGS(m_resource.GetAddressOf())
    ));

    // In order to copy CPU memory data into our default buffer,
    // we need to create an intermediate upload heap.
    DXCALL(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(size),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&uploadBuffer)
    ));

    D3D12_SUBRESOURCE_DATA sub_resource_data = {};
    sub_resource_data.pData = data;
    sub_resource_data.RowPitch = size;
    sub_resource_data.SlicePitch = sub_resource_data.RowPitch;

    // Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
    // will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
    // the intermediate upload heap data will be copied to mBuffer.
    this->Transition(D3D12_RESOURCE_STATE_COPY_DEST);
    UpdateSubresources<1>(cmd_list->GetApiCommandList(), m_resource.Get(), uploadBuffer, 0, 0, 1, &sub_resource_data);
    this->Transition(D3D12_RESOURCE_STATE_GENERIC_READ);
}

Dx12Resource::~Dx12Resource()
{
    m_resource.Reset();
}

ID3D12Resource* Dx12Resource::GetApiResource() const
{
    return m_resource.Get();
}

ID3D12Resource* const* Dx12Resource::GetApiResourceAddress() const
{
    return m_resource.GetAddressOf();
}

ID3D12Resource** Dx12Resource::GetApiResourceAddress()
{
    return m_resource.GetAddressOf();
}

D3D12_RESOURCE_STATES Dx12Resource::GetCurrentState() const
{
    return m_current_state;
}

void Dx12Resource::ForceSetResourceState(const D3D12_RESOURCE_STATES& state)
{
    spdlog::warn("Force setting resource state of resource!");
    m_current_state = state;
}

void Dx12Resource::Transition(D3D12_RESOURCE_STATES newState)
{
    //auto direct_cmd_list = GetDx12Context()->CreateDirectCommandList();
    //direct_cmd_list->Push<Dx12SwitchResourceStateCommand>(this, newState);
    GetDx12CommandList()->GetApiCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_resource.Get(), m_current_state, newState));
    m_current_state = newState;
}

