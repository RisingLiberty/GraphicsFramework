#pragma once

#include "Command.h"

class ClearDepthStencilCommand : public Command
{
public:
	ClearDepthStencilCommand();
	virtual ~ClearDepthStencilCommand();

	void Execute() override = 0;

	virtual std::string ToString() const override;
};