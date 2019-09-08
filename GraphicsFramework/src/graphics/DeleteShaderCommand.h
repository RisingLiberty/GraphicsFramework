#pragma once

#include "Command.h"

class DeleteShaderCommand : public Command
{
public:
	DeleteShaderCommand();
	virtual ~DeleteShaderCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override = 0;
};