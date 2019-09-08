#pragma once

#include "Command.h"

class ApiBufferWrapper;

class UnmapBufferCommand : public Command
{
public: 
	UnmapBufferCommand(const ApiBufferWrapper* buffer);
	virtual ~UnmapBufferCommand();

	virtual std::string ToString() const override;

protected:
	const ApiBufferWrapper* m_buffer;
};