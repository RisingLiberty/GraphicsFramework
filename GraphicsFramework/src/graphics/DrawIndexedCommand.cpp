#include "stdafx.h"

#include "DrawIndexedCommand.h"

DrawIndexedCommand::DrawIndexedCommand(unsigned int count, Topology topology, Format format):
	m_count(count),
	m_topology(topology),
	m_format(format)
{

}

DrawIndexedCommand::~DrawIndexedCommand()
{

}

std::string DrawIndexedCommand::ToString() const
{
	return "Draw indexed";
}
