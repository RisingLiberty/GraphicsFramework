#pragma once

#include "Command.h"
#include "BufferHelpers.h"

class ApiBufferWrapper;

class SetBufferDataCommand : public Command
{
public:
	SetBufferDataCommand(ApiBufferWrapper* buffer, unsigned int size, const void* data, BufferUsage usage);
	virtual ~SetBufferDataCommand();

	virtual void Execute() = 0;

protected:
	ApiBufferWrapper* m_buffer;
	unsigned int m_size;
	void* m_data;
	BufferUsage m_usage;
};