#include "stdafx.h"

#include "Dx12SetViewportCommand.h"

#include "../Dx12HelperMethods.h"

Dx12SetViewportCommand::Dx12SetViewportCommand(const D3D12_VIEWPORT& viewport) :
    m_viewport(viewport)
{

}

Dx12SetViewportCommand::~Dx12SetViewportCommand()
{

}

std::string Dx12SetViewportCommand::ToString() const
{
    return "Set viewport";
}

void Dx12SetViewportCommand::Execute()
{
    GetDx12CommandList()->GetApiCommandList()->RSSetViewports(1, &m_viewport);
}