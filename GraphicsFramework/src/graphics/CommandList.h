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

	void Open();
	void Close();

	void Execute();

private:
	std::queue<std::unique_ptr<Command>> m_commands;
	bool m_is_open;
};