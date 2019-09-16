#pragma once

#include "graphics/commands/DrawIndexedCommand.h"

class Dx11DrawIndexedCommand : public DrawIndexedCommand
{
public:
	Dx11DrawIndexedCommand(const unsigned int count);
	virtual ~Dx11DrawIndexedCommand();

	void Execute() override;
};