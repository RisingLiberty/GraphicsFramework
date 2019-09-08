#include "stdafx.h"

#include "BindCopySrcBufferCommand.h"

BindCopySrcBufferCommand::BindCopySrcBufferCommand(const ApiBufferWrapper* buffer):
	m_buffer(buffer)
{

}

BindCopySrcBufferCommand::~BindCopySrcBufferCommand()
{

}

std::string BindCopySrcBufferCommand::ToString() const
{
	return "Bind copy source buffer";
}