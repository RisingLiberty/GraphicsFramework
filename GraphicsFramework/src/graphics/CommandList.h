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

	virtual void Execute();

    bool IsOpen() const;

protected:
	std::queue<std::unique_ptr<Command>> m_commands;
	bool m_is_open;
};