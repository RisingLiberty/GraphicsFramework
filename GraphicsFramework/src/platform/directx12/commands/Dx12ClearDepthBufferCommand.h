#pragma once

#include "graphics/commands/ClearDepthBufferCommand.h"

class Dx12ClearDepthBufferCommand : public ClearDepthBufferCommand
{
public:
    Dx12ClearDepthBufferCommand(const float depthValue, const D3D12_CPU_DESCRIPTOR_HANDLE& stencilBufferView);
    virtual ~Dx12ClearDepthBufferCommand();

    void Execute() override;

private:
    const D3D12_CPU_DESCRIPTOR_HANDLE m_depth_buffer_view;
};