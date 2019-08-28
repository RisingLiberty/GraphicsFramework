#pragma once

#include "Command.h"

class SetRenderTargetCommand : public Command
{
public:
	SetRenderTargetCommand();
	virtual ~SetRenderTargetCommand();

	void Execute() = 0;
};