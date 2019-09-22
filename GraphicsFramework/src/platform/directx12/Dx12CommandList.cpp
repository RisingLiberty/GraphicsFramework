#include "stdafx.h"

#include "Dx12HelperMethods.h"
#include "Dx12CommandList.h"

#include "commands/Dx12BindIndexBufferCommand.h"
#include "commands/Dx12BindVertexArrayCommand.h"
#include "commands/Dx12DrawIndexedCommand.h"
#include "commands/Dx12SetDescriptorHeapsCommand.h"
#include "commands/Dx12SetGraphicsRootDescTableCommand.h"
#include "commands/Dx12SetGraphicsRootSignatureCommand.h"
#include "commands/Dx12SetPipelineStateCommand.h"
#include "commands/Dx12SetRenderTargetCommand.h"
#include "commands/Dx12SetScissorRectCommand.h"
#include "commands/Dx12SetViewportCommand.h"
#include "commands/Dx12SwitchResourceStateCommand.h"

#include "Dx12Resource.h"

Dx12CommandList::Dx12CommandList()
{
	DXCALL(GetDx12Device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_cmd_alloc.ReleaseAndGetAddressOf())));
	DXCALL(GetDx12Device()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmd_alloc.Get(), nullptr, IID_PPV_ARGS(m_list.ReleaseAndGetAddressOf())));
}

Dx12CommandList::~Dx12CommandList()
{

}

void Dx12CommandList::Open()
{
    CommandList::Open();
    if (!m_pipeline_state.Get())
        spdlog::warn("Resetting command list with empty pipeline state!");

    m_list->Reset(m_cmd_alloc.Get(), m_pipeline_state.Get());
}

void Dx12CommandList::Close()
{
    CommandList::Close();
    //m_list->Close();
}

void Dx12CommandList::ResetAlloc()
{
	m_cmd_alloc->Reset();
}

ID3D12GraphicsCommandList* Dx12CommandList::GetApiCommandList() const
{
	return m_list.Get();
}