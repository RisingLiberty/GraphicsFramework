#pragma once

#include "graphics/commands/ClearStencilBufferCommand.h"

class Dx12ClearStencilBufferCommand : public ClearStencilBufferCommand
{
public:
    Dx12ClearStencilBufferCommand(const unsigned int stencilValue, const D3D12_CPU_DESCRIPTOR_HANDLE& stencilBufferView);
    virtual ~Dx12ClearStencilBufferCommand();

    void Execute() override;

private:
    const D3D12_CPU_DESCRIPTOR_HANDLE m_stencil_buffer_view;
};