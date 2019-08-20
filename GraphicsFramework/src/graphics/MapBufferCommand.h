#pragma once

#include "Command.h"

class ApiBufferWrapper;

class MapBufferCommand : public Command
{
public:
	MapBufferCommand(const ApiBufferWrapper* buffer);
	virtual ~MapBufferCommand();

private:
	const ApiBufferWrapper* m_buffer;
};