#pragma once

#include "Command.h"

class ApiBufferWrapper;

class CopyBufferCommand : public Command
{
public:
	CopyBufferCommand(unsigned int size, const ApiBufferWrapper* destination, const ApiBufferWrapper* source);
	virtual ~CopyBufferCommand();

	void Execute() override = 0;
	virtual std::string ToString() const override;

protected:
	unsigned int m_size;
	const ApiBufferWrapper* m_destination;
	const ApiBufferWrapper* m_source;
};