#include "stdafx.h"

#include "BindVertexBufferCommand.h"

BindVertexBufferCommand::BindVertexBufferCommand(const VertexBuffer* vb) :
	m_vertex_buffer(vb)
{
}

BindVertexBufferCommand::~BindVertexBufferCommand()
{

}