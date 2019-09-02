#pragma once

#include "Command.h"

class CreateBufferCommand : public Command
{
public: 
	CreateBufferCommand();
	virtual ~CreateBufferCommand();

	virtual void Execute() override = 0;
	virtual std::string ToString() const override;
};