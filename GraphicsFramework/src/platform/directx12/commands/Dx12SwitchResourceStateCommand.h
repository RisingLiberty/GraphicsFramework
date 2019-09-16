#pragma once

#include "graphics/commands/Command.h"

class Dx12Resource;

class Dx12SwitchResourceStateCommand : public Command
{
public:
    Dx12SwitchResourceStateCommand(Dx12Resource* resource, D3D12_RESOURCE_STATES resourceState);
    virtual ~Dx12SwitchResourceStateCommand();

    std::string ToString() const override;
    void Execute() override;

private:
    Dx12Resource* m_resource;
    D3D12_RESOURCE_STATES m_resource_state;
};