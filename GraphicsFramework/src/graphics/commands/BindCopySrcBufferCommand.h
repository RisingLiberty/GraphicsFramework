#pragma once

#include "Command.h"

class ApiBufferWrapper;

class BindCopySrcBufferCommand : public Command
{
public:
	BindCopySrcBufferCommand(const ApiBufferWrapper* buffer);
	virtual ~BindCopySrcBufferCommand();

	virtual void Execute() override = 0;
	virtual std::string ToString() const override;

protected:
	const ApiBufferWrapper* m_buffer;
};