#include "stdafx.h"

#include "DrawIndexedCommand.h"

DrawIndexedCommand::DrawIndexedCommand(const unsigned int count):
	m_count(count)
{

}

DrawIndexedCommand::~DrawIndexedCommand()
{

}

std::string DrawIndexedCommand::ToString() const
{
	return "Draw indexed";
}
