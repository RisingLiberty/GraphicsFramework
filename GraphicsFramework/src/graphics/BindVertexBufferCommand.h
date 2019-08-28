#pragma once

#include "Command.h"

class VertexBuffer;

class BindVertexBufferCommand : public Command
{
public:
	BindVertexBufferCommand(const VertexBuffer* vb);
	virtual ~BindVertexBufferCommand();

	virtual void Execute() = 0;

protected:
	const VertexBuffer* m_vertex_buffer;
};