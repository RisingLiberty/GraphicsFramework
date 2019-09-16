#include "stdafx.h"

#include "Dx12SetRenderTargetCommand.h"
#include "../Dx12HelperMethods.h"

Dx12SetRenderTargetCommand::Dx12SetRenderTargetCommand(const D3D12_CPU_DESCRIPTOR_HANDLE& renderTargetView, const D3D12_CPU_DESCRIPTOR_HANDLE& depthStencilView) :
    m_render_target_view(renderTargetView),
    m_depth_stencil_view(depthStencilView)
{

}

Dx12SetRenderTargetCommand::~Dx12SetRenderTargetCommand()
{

}

std::string Dx12SetRenderTargetCommand::ToString() const
{
    return "Set render target";
}

void Dx12SetRenderTargetCommand::Execute()
{
    GetDx12CommandList()->GetApiCommandList()->OMSetRenderTargets(1, &m_render_target_view, true, &m_depth_stencil_view);
}