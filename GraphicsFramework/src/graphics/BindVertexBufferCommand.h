#pragma once

#include "Command.h"

class VertexBuffer;

class BindVertexBufferCommand : Command
{
public:
	BindVertexBufferCommand(const VertexBuffer* vb);
	virtual ~BindVertexBufferCommand();

	virtual void Execute() = 0;

private:
	const VertexBuffer* m_vertex_buffer;
};