#include "stdafx.h"

#include "Dx12SetScissorRectCommand.h"

#include "../Dx12HelperMethods.h"

Dx12SetScissorRectCommand::Dx12SetScissorRectCommand(const D3D12_RECT& rect) :
    m_rect(rect)
{

}

Dx12SetScissorRectCommand::~Dx12SetScissorRectCommand()
{

}

std::string Dx12SetScissorRectCommand::ToString() const
{
    return "Set scissor rect";
}

void Dx12SetScissorRectCommand::Execute()
{
    GetDx12CommandList()->GetApiCommandList()->RSSetScissorRects(1, &m_rect);
}