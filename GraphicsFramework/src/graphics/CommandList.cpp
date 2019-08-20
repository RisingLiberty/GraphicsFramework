#include "stdafx.h"

#include "CommandList.h"
#include "Command.h"

CommandList::CommandList():
	m_is_open(false)
{
}

CommandList::~CommandList()
{

}

void CommandList::Open()
{
	m_is_open = true;
}

void CommandList::Close()
{
	m_is_open = false;
}

void CommandList::Execute()
{
	if (m_is_open)
	{
		spdlog::error("Trying to execute command list that's still open!");
		return;
	}

	while (!m_commands.empty())
	{
		std::unique_ptr<Command>& command = m_commands.front();
		m_commands.pop();

		command->Execute();
	}
}