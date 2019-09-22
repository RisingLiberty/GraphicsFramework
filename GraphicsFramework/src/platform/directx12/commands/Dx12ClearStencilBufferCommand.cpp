#include "stdafx.h"

#include "Dx12ClearStencilBufferCommand.h"

#include "../Dx12HelperMethods.h"

Dx12ClearStencilBufferCommand::Dx12ClearStencilBufferCommand(const unsigned int stencilValue, const D3D12_CPU_DESCRIPTOR_HANDLE& stencilBufferView) :
    ClearStencilBufferCommand(stencilValue),
    m_stencil_buffer_view(stencilBufferView)
{

}

Dx12ClearStencilBufferCommand::~Dx12ClearStencilBufferCommand()
{

}

void Dx12ClearStencilBufferCommand::Execute()
{
    GetDx12CommandList()->GetApiCommandList()->ClearDepthStencilView(m_stencil_buffer_view, D3D12_CLEAR_FLAG_STENCIL, 1.0f, m_stencil_value, 0, nullptr);
}