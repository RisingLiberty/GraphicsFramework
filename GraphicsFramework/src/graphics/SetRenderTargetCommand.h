#pragma once

#include "Command.h"

class SetRenderTargetCommand : public Command
{
public:
	SetRenderTargetCommand();
	virtual ~SetRenderTargetCommand();

	void Execute() override = 0;
	virtual std::string ToString() const override;
};