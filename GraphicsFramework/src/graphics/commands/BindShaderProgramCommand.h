#pragma once

#include "Command.h"

class BindShaderProgramCommand : public Command
{
public:
	BindShaderProgramCommand();
	virtual ~BindShaderProgramCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override = 0;
};