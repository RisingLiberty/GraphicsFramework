#include "stdafx.h"

#include "Dx12BindVertexArrayCommand.h"

#include "../Dx12HelperMethods.h"

#include "../Dx12VertexArray.h"
#include "../Dx12VertexBuffer.h"

Dx12BindVertexArrayCommand::Dx12BindVertexArrayCommand(const VertexArray* va) :
    BindVertexArrayCommand(va)
{

}

Dx12BindVertexArrayCommand::~Dx12BindVertexArrayCommand()
{

}

void Dx12BindVertexArrayCommand::Execute()
{
    GetDx12CommandList()->GetApiCommandList()->IASetVertexBuffers(0, 1, &m_vertex_array->As<Dx12VertexArray>()->GetVertexBufferView());
}