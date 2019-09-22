#pragma once

#include "graphics/commands/ClearDepthStencilBufferCommand.h"

class Dx12ClearDepthStencilBufferCommand : public ClearDepthStencilBufferCommand
{
public:
    Dx12ClearDepthStencilBufferCommand(const float depthValue, const unsigned int stencilValue, const D3D12_CPU_DESCRIPTOR_HANDLE& depthStencilView);
    virtual ~Dx12ClearDepthStencilBufferCommand();

    void Execute() override;

private:
    const D3D12_CPU_DESCRIPTOR_HANDLE m_depth_stencil_view;
};