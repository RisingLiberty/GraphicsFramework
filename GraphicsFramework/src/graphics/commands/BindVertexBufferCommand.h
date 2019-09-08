#pragma once

#include "Command.h"

class VertexBuffer;

class BindVertexBufferCommand : public Command
{
public:
	BindVertexBufferCommand(const VertexBuffer* vb);
	virtual ~BindVertexBufferCommand();

	virtual void Execute() override = 0;
	virtual std::string ToString() const override;

protected:
	const VertexBuffer* m_vertex_buffer;
};