#include "stdafx.h"

#include "Dx12ClearRenderTargetCommand.h"

#include "../Dx12HelperMethods.h"
#include "../Dx12Resource.h"

Dx12ClearRenderTargetCommand::Dx12ClearRenderTargetCommand(const std::array<float, 4>& clearColor, const D3D12_CPU_DESCRIPTOR_HANDLE& renderTargetView) :
    ClearRenderTargetCommand(clearColor),
    m_render_target_view(renderTargetView)
{

}

Dx12ClearRenderTargetCommand::~Dx12ClearRenderTargetCommand()
{

}

void Dx12ClearRenderTargetCommand::Execute()
{
    GetDx12CommandList()->GetApiCommandList()->ClearRenderTargetView(m_render_target_view, m_clear_color.data(), 0, nullptr);
}