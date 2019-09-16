#pragma once

#include "graphics/commands/Command.h"

class Dx12SetRenderTargetCommand : public Command
{
public:
    Dx12SetRenderTargetCommand(const D3D12_CPU_DESCRIPTOR_HANDLE& renderTargetView, const D3D12_CPU_DESCRIPTOR_HANDLE& depthStencilView);
    virtual ~Dx12SetRenderTargetCommand();

    std::string ToString() const override;
    void Execute() override;

private:
    const D3D12_CPU_DESCRIPTOR_HANDLE m_render_target_view;
    const D3D12_CPU_DESCRIPTOR_HANDLE m_depth_stencil_view;

};