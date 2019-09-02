#include "stdafx.h"

#include "BindCopyDestBufferCommand.h"

BindCopyDestBufferCommand::BindCopyDestBufferCommand(const ApiBufferWrapper* buffer):
	m_buffer(buffer)
{

}

BindCopyDestBufferCommand::~BindCopyDestBufferCommand()
{

}

std::string BindCopyDestBufferCommand::ToString() const
{
	return "Bind copy destination buffer";
}