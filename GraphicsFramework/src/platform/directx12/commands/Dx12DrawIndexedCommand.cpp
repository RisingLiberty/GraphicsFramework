#include "stdafx.h"

#include "Dx12DrawIndexedCommand.h"

#include "../Dx12HelperMethods.h"

Dx12DrawIndexedCommand::Dx12DrawIndexedCommand(const unsigned int count) :
    DrawIndexedCommand(count)
{

}

Dx12DrawIndexedCommand::~Dx12DrawIndexedCommand()
{

}

void Dx12DrawIndexedCommand::Execute()
{
    GetDx12CommandList()->GetApiCommandList()->DrawIndexedInstanced(m_count, 1, 0, 0, 0);
}