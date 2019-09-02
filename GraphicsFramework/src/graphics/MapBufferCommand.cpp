#include "stdafx.h"

#include "MapBufferCommand.h"

MapBufferCommand::MapBufferCommand(const ApiBufferWrapper* buffer, void** data) :
	m_buffer(buffer),
	m_data(data)
{

}

MapBufferCommand::~MapBufferCommand()
{

}

std::string MapBufferCommand::ToString() const
{
	return "Map buffer";
}
