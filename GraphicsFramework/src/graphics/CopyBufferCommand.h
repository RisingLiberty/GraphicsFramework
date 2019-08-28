#pragma once

#include "Command.h"

class CopyBufferCommand : public Command
{
public:
	CopyBufferCommand();
	virtual ~CopyBufferCommand();

	void Execute() = 0;
};