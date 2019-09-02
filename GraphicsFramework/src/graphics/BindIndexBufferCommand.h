#pragma once

#include "Command.h"

class IndexBuffer;

class BindIndexBufferCommand : public Command
{
public:
	BindIndexBufferCommand(const IndexBuffer* ib);
	virtual ~BindIndexBufferCommand();

	virtual void Execute() override = 0;
	virtual std::string ToString() const override;

protected:
	const IndexBuffer* m_index_buffer;
};