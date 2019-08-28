#pragma once

#include "graphics/SetBufferDataCommand.h"

class GLSetBufferDataCommand : public SetBufferDataCommand
{
public:
	GLSetBufferDataCommand(ApiBufferWrapper* buffer, unsigned int size, const void* data, BufferUsage usage, BufferType type);
	virtual ~GLSetBufferDataCommand();

	virtual void Execute();

private:
	BufferType m_type;
};