#pragma once

#include "graphics/commands/ClearRenderTargetCommand.h"

class Dx12Resource;

class Dx12ClearRenderTargetCommand : public ClearRenderTargetCommand
{
public:
    Dx12ClearRenderTargetCommand(const std::array<float, 4>& clearColor, const D3D12_CPU_DESCRIPTOR_HANDLE& renderTargetView);
    virtual ~Dx12ClearRenderTargetCommand();

    void Execute() override;

private:
    D3D12_CPU_DESCRIPTOR_HANDLE m_render_target_view;
};