#pragma once

#include "Command.h"

class ClearStencilBufferCommand : public Command
{
public:
	ClearStencilBufferCommand(const unsigned int stencilValue);
	virtual ~ClearStencilBufferCommand();

	virtual void Execute() override = 0;
	virtual std::string ToString() const override;

protected:
	const unsigned int m_stencil_value;
};