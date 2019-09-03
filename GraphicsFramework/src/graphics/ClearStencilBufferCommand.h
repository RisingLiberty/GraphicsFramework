#pragma once

#include "Command.h"

class ClearStencilBufferCommand : public Command
{
public:
	ClearStencilBufferCommand(unsigned int stencilValue);
	virtual ~ClearStencilBufferCommand();

	virtual void Execute() override = 0;
	virtual std::string ToString() const override;

protected:
	unsigned int m_stencil_value;
};