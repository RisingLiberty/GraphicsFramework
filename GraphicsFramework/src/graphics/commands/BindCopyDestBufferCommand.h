#pragma once

#include "Command.h"

class ApiBufferWrapper;

class BindCopyDestBufferCommand : public Command
{
public:
	BindCopyDestBufferCommand(const ApiBufferWrapper* buffer);
	virtual ~BindCopyDestBufferCommand();

	virtual void Execute() override = 0;
	virtual std::string ToString() const override;

protected:
	const ApiBufferWrapper* m_buffer;

};