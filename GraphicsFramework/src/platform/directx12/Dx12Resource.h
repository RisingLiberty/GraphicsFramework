#pragma once

#include "graphics/GraphicsResource.h"

class Dx12Resource : public GraphicsResource<Dx12Resource>
{
public:
    Dx12Resource(ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES resourceState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON);
    Dx12Resource(unsigned int size, const void* data, ID3D12Resource* uploadBuffer);
    virtual ~Dx12Resource();

    ID3D12Resource* GetApiResource() const;
    ID3D12Resource* const* GetApiResourceAddress() const;
    ID3D12Resource** GetApiResourceAddress();
    D3D12_RESOURCE_STATES GetCurrentState() const;

    void Transition(D3D12_RESOURCE_STATES newState);

private:
    ComPtr<ID3D12Resource> m_resource;
    D3D12_RESOURCE_STATES m_current_state;
};