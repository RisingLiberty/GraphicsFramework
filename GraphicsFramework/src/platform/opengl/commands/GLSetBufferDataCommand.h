#pragma once

#include "graphics/commands/SetBufferDataCommand.h"

class GLSetBufferDataCommand : public SetBufferDataCommand
{
public:
	GLSetBufferDataCommand(ApiBufferWrapper* buffer, unsigned int size, const void* data, BufferType type);
	virtual ~GLSetBufferDataCommand();

	virtual void Execute();

private:
	BufferType m_type;
};