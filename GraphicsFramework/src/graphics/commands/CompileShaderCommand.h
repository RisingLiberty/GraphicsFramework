#pragma once

#include "Command.h"

class CompileShaderCommand : public Command
{
public:
	CompileShaderCommand();
	virtual ~CompileShaderCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override = 0;
};