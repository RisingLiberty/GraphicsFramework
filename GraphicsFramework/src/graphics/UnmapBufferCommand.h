#pragma once

#include "Command.h"

class ApiBufferWrapper;

class UnmapBufferCommand : public Command
{
public: 
	UnmapBufferCommand(const ApiBufferWrapper* buffer);
	virtual ~UnmapBufferCommand();

private:
	const ApiBufferWrapper* m_buffer;
};