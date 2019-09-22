#include "stdafx.h"

#include "Dx12ClearDepthStencilBufferCommand.h"

#include "../Dx12HelperMethods.h"

Dx12ClearDepthStencilBufferCommand::Dx12ClearDepthStencilBufferCommand(const float depthValue, const unsigned int clearValue, const D3D12_CPU_DESCRIPTOR_HANDLE& depthStencilView) :
    ClearDepthStencilBufferCommand(depthValue, clearValue),
    m_depth_stencil_view(depthStencilView)
{

}

Dx12ClearDepthStencilBufferCommand::~Dx12ClearDepthStencilBufferCommand()
{

}

void Dx12ClearDepthStencilBufferCommand::Execute()
{
    GetDx12CommandList()->GetApiCommandList()->ClearDepthStencilView(m_depth_stencil_view, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, m_depth_value, m_stencil_value, 0, nullptr);
}