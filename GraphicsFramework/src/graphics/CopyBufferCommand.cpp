#include "stdafx.h"

#include "CopyBufferCommand.h"

CopyBufferCommand::CopyBufferCommand(unsigned int size, const ApiBufferWrapper* destination, const ApiBufferWrapper* source):
	m_size(size),
	m_destination(destination),
	m_source(source)
{

}

CopyBufferCommand::~CopyBufferCommand()
{

}

std::string CopyBufferCommand::ToString() const
{
	return "Copy buffer";
}
