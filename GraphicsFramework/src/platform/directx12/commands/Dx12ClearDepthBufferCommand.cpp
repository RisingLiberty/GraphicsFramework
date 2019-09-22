#include "stdafx.h"

#include "Dx12ClearDepthBufferCommand.h"

#include "../Dx12HelperMethods.h"

Dx12ClearDepthBufferCommand::Dx12ClearDepthBufferCommand(const float depthValue, const D3D12_CPU_DESCRIPTOR_HANDLE& stencilBufferView) :
    ClearDepthBufferCommand(depthValue),
    m_depth_buffer_view(stencilBufferView)
{

}

Dx12ClearDepthBufferCommand::~Dx12ClearDepthBufferCommand()
{

}

void Dx12ClearDepthBufferCommand::Execute()
{
    GetDx12CommandList()->GetApiCommandList()->ClearDepthStencilView(m_depth_buffer_view, D3D12_CLEAR_FLAG_DEPTH, m_depth_value, 0, 0, nullptr);
}