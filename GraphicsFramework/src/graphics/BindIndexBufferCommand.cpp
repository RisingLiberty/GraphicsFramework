#include "stdafx.h"

#include "BindIndexBufferCommand.h"

BindIndexBufferCommand::BindIndexBufferCommand(const IndexBuffer* ib):
	m_index_buffer(ib)
{

}

BindIndexBufferCommand::~BindIndexBufferCommand()
{

}