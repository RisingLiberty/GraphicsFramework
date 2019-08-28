#pragma once

#include "Graphics/DeleteBufferCommand.h"

class GLDeleteBufferCommand : public DeleteBufferCommand
{
public:
	GLDeleteBufferCommand(unsigned int bufferId);
	virtual ~GLDeleteBufferCommand();

	virtual void Execute();

private:
	unsigned int m_buffer_id;
};