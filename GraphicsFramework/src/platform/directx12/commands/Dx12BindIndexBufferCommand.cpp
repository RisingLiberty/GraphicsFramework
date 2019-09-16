#include "stdafx.h"

#include "Dx12BindIndexBufferCommand.h"

#include "../Dx12HelperMethods.h"
#include "../Dx12IndexBuffer.h"

Dx12BindIndexBufferCommand::Dx12BindIndexBufferCommand(const IndexBuffer* ib) :
    BindIndexBufferCommand(ib)
{

}

Dx12BindIndexBufferCommand::~Dx12BindIndexBufferCommand()
{

}

void Dx12BindIndexBufferCommand::Execute()
{
    GetDx12CommandList()->GetApiCommandList()->IASetPrimitiveTopology(m_index_buffer->GetTopology().ToDirectX());
    GetDx12CommandList()->GetApiCommandList()->IASetIndexBuffer(&m_index_buffer->As<Dx12IndexBuffer>()->GetBufferView());
}