#pragma once

#include "graphics/commands/Command.h"

class Dx12SetViewportCommand : public Command
{
public:
    Dx12SetViewportCommand(const D3D12_VIEWPORT& viewport);
    virtual ~Dx12SetViewportCommand();

    void Execute() override;

private:
    const D3D12_VIEWPORT m_viewport;
};