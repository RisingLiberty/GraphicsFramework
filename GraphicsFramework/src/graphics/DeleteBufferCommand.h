#pragma once

#include "Command.h"

class DeleteBufferCommand : public Command
{
public:
	DeleteBufferCommand();
	virtual ~DeleteBufferCommand();

	virtual void Execute() override = 0;
};