#include "stdafx.h"

#include "CommandList.h"
#include "commands/Command.h"

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

void CommandList::Push(std::unique_ptr<Command> command)
{
	//ASSERT(m_is_open, "Trying to push a command on a closed command list");
	m_commands.push(std::move(command));

    if (m_should_log_commands)
    	spdlog::info("{} command added", m_commands.back()->ToString());
}

void CommandList::Execute()
{
	if (m_is_open)
	{
		spdlog::error("Trying to execute command list that's still open!");
		//return;
	}

	while (!m_commands.empty())
	{
		std::unique_ptr<Command>& command = m_commands.front();
		command->Execute();

		m_commands.pop();
	}
}

bool CommandList::IsOpen() const
{
    return m_is_open;
}

void CommandList::EnableCommandLogging()
{
    m_should_log_commands = true;
}

void CommandList::DisableCommandLogging()
{
    m_should_log_commands = false;
}