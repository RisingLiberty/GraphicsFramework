#include "stdafx.h"

#include "Dx12SwitchResourceStateCommand.h"

#include "../Dx12HelperMethods.h"
#include "../Dx12Resource.h"

Dx12SwitchResourceStateCommand::Dx12SwitchResourceStateCommand(Dx12Resource* resource, D3D12_RESOURCE_STATES resourceState) :
    m_resource(resource),
    m_resource_state(resourceState)
{

}

Dx12SwitchResourceStateCommand::~Dx12SwitchResourceStateCommand()
{

}

std::string Dx12SwitchResourceStateCommand::ToString() const
{
    return "Switch resource state";
}

void Dx12SwitchResourceStateCommand::Execute()
{
    GetDx12CommandList()->GetApiCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_resource->GetApiResource(), m_resource->GetCurrentState(), m_resource_state));
}