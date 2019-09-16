#include "stdafx.h"

#include "MapBufferCommand.h"

MapBufferCommand::MapBufferCommand(const ApiBufferWrapper* buffer) :
	m_buffer(buffer)
{

}

MapBufferCommand::~MapBufferCommand()
{

}

std::string MapBufferCommand::ToString() const
{
	return "Map buffer";
}
