#pragma once

#include "Command.h"

class ClearRenderTargetCommand : public Command
{
public:
	ClearRenderTargetCommand();
	virtual ~ClearRenderTargetCommand();

	void Execute() override = 0;
	virtual std::string ToString() const override;
};