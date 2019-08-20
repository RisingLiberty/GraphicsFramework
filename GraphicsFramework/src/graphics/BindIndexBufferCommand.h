#pragma once

#include "Command.h"

class IndexBuffer;

class BindIndexBufferCommand : public Command
{
public:
	BindIndexBufferCommand(const IndexBuffer* ib);
	virtual ~BindIndexBufferCommand();

	virtual void Execute() = 0;

protected:
	const IndexBuffer* m_index_buffer;
};