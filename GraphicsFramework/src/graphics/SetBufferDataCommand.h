#pragma once

#include "Command.h"
#include "BufferHelpers.h"

class ApiBufferWrapper;

class SetBufferDataCommand : public Command
{
public:
	SetBufferDataCommand(ApiBufferWrapper* buffer, unsigned int size, const void* data);
	virtual ~SetBufferDataCommand();

	virtual void Execute() override = 0;
	virtual std::string ToString() const override;

protected:
	ApiBufferWrapper* m_buffer;
	unsigned int m_size;
	void* m_data;
};