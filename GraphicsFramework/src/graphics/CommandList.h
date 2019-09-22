#pragma once

#include "GraphicsResource.h"

class IndexBuffer;
class VertexBuffer;

class Command;

class CommandList : public GraphicsResource<CommandList>
{
public:
	CommandList();
	virtual ~CommandList();

	virtual void Open();
	virtual void Close();

    void Push(std::unique_ptr<Command> command);

    template <typename T, typename... T2>
    void Push(T2... args)
    {
        //ASSERT(m_is_open, "Trying to push a command on a closed command list");
        m_commands.push(std::make_unique<T>(args...));

        if (m_should_log_commands)
            spdlog::info("{} command added", m_commands.back()->ToString());
    }

	virtual void Execute();

    bool IsOpen() const;

    void EnableCommandLogging();
    void DisableCommandLogging();

protected:
	std::queue<std::unique_ptr<Command>> m_commands;
	bool m_is_open;
   
private:
    bool m_should_log_commands;
};