#pragma once

#include "Command.h"

class BindVertexLayoutCommand : public Command
{
public:
	BindVertexLayoutCommand();
	virtual ~BindVertexLayoutCommand();

	void Execute() = 0;
};